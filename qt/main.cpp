#include <QAction>
#include <QApplication>
#include <QCheckBox>
#include <QColor>
#include <QColorDialog>
#include <QCoreApplication>
#include <QDialog>
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QKeySequence>
#include <QFont>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QRadioButton>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QMouseEvent>
#include <QOpenGLWidget>
#include <QPushButton>
#include <QStatusBar>
#include <QSurfaceFormat>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWheelEvent>
#include <QWidget>
#include <cstring>
#include <cmath>
#include <vector>

#if defined(Q_OS_UNIX)
#include <unistd.h>
#endif

#include "../optmesh/smd.h"
#include "../optmesh/ppd.h"
#include "../optmesh/ppdlist.h"
#include "../optmesh/params.h"
#include "../optmesh/qem.h"
#include "../optmesh/simpmesh.h"
#include "../optmesh/subdiv.h"
#include "../optmesh/mytime.h"
#include "../src/screen.h"
#include "../src/draw.h"
#include "../src/meshtoss_gui_prefix.h"
#include "../src/mtl.h"
#include "../src/smf.h"
#include "../src/swin.h"
#include "../src/vw.h"

namespace {

/* After simpmesh / simpmesh_l2norm, includes processed time (Unix). Windows build has no timers yet. */
static QString conversionFinishedMessage()
{
#if defined(Q_OS_WIN)
  return QStringLiteral("Conversion successfully finished.");
#else
  double r = 0.0;
  double u = 0.0;
  double s = 0.0;
  mytime_get_last(&r, &u, &s);
  return QStringLiteral("Conversion successfully finished.\n\n"
                        "processed time:\n"
                        "\treal:\t%1 (s)\n"
                        "\tuser:\t%2 (s)\n"
                        "\tsys:\t%3 (s)")
      .arg(r, 0, 'f', 2)
      .arg(u, 0, 'f', 2)
      .arg(s, 0, 'f', 2);
#endif
}

/* Set in main() before QApplication — Qt/macOS often resets CWD to "/" after the GUI starts. */
QString g_fileDialogBaseDir;

/**
 * Call once at program entry, before QApplication(argc, argv).
 * Uses getcwd first, then $PWD if the result is "/" (common for .app / Qt on macOS).
 */
void initFileDialogBaseDirEarly()
{
  QString dir;
#if defined(Q_OS_UNIX)
  char buf[4096];
  if (getcwd(buf, sizeof(buf)) != nullptr) {
    dir = QString::fromLocal8Bit(buf);
  }
#endif
  if (dir.isEmpty()) {
    dir = QDir::currentPath();
  }
  dir = QDir(dir).canonicalPath();
  if (dir.isEmpty()) {
    dir = QDir::currentPath();
  }

#if defined(Q_OS_UNIX)
  if (dir == QLatin1String("/")) {
    const QByteArray pwd = qgetenv("PWD");
    if (!pwd.isEmpty()) {
      QFileInfo fi(QString::fromLocal8Bit(pwd));
      if (fi.exists() && fi.isDir()) {
        const QString c = fi.canonicalFilePath();
        if (!c.isEmpty()) {
          dir = c;
        }
      }
    }
  }
#endif

#if defined(Q_OS_DARWIN)
  if (dir == QLatin1String("/")) {
    const QString home = QDir::homePath();
    if (!home.isEmpty()) {
      dir = home;
    }
  }
#endif

  g_fileDialogBaseDir = dir;
}

/** Initial path for QFileDialog; save dialogs use basename of \a suggestedFile under the captured base. */
QString fileDialogStartPath(const QString &suggestedFile = QString())
{
  const QString base =
      g_fileDialogBaseDir.isEmpty() ? QDir::currentPath() : g_fileDialogBaseDir;
  if (suggestedFile.isEmpty()) {
    return base;
  }
  return QDir(base).filePath(QFileInfo(suggestedFile).fileName());
}

void installEmptyBitmapFontLists(ScreenAtr *screen)
{
  const GLuint base = 1000;
  screen->fontOffset = base;
  for (GLuint i = 0; i < 256; ++i) {
    glNewList(base + i, GL_COMPILE);
    glEndList();
  }
}

void syncScreenSize(ScreenAtr *screen, int w, int h)
{
  if (w < 1) {
    w = 1;
  }
  if (h < 1) {
    h = 1;
  }
  screen->width = w;
  screen->height = h;
  screen->xcenter = static_cast<double>(w) / 2.0;
  screen->ycenter = static_cast<double>(h) / 2.0;
}

void fitViewToPpd(ScreenAtr *screen, Sppd *ppd)
{
  if (ppd == nullptr || ppd->spvt == nullptr) {
    set_default_view(screen);
    return;
  }

  double maxx = 0.0;
  double minx = 0.0;
  double maxy = 0.0;
  double miny = 0.0;
  double maxz = 0.0;
  double minz = 0.0;
  ppd_size(ppd, &maxx, &minx, &maxy, &miny, &maxz, &minz);

  const double cx = 0.5 * (maxx + minx);
  const double cy = 0.5 * (maxy + miny);
  const double cz = 0.5 * (maxz + minz);
  const double dx = maxx - minx;
  const double dy = maxy - miny;
  const double dz = maxz - minz;
  double diag = std::sqrt(dx * dx + dy * dy + dz * dz);
  if (diag < 1e-30) {
    diag = 1.0;
  }

  screen->pers.ref.x = cx;
  screen->pers.ref.y = cy;
  screen->pers.ref.z = cz;

  screen->translate_vec.x = 0.0;
  screen->translate_vec.y = 0.0;
  screen->translate_vec.z = 0.0;

  screen->zoom = 1.0;
  screen->rotate_x = 0.0;
  screen->rotate_y = 0.0;
  screen->rotate_z = 0.0;
  screen->rotate_type = ROTATE_XY;
  screen->zrot_rad = 0.0;
  calc_rotate_matrix(screen);

  const double halfAngleRad = screen->pers.angle * SMDPI / 360.0;
  const double tanHalf = std::tan(halfAngleRad);
  if (tanHalf < 1e-10) {
    screen->pers.fd = diag * 1.5;
  } else {
    screen->pers.fd = (0.5 * diag) / tanHalf * 1.15;
  }

  screen->pers.znear_ratio = 0.0001;
  screen->pers.zfar_ratio = 10000.0;
}

void attachLoadedPpd(Sppd *ppd)
{
  copy_material(&(ppd->matl), 0, static_cast<float *>(mtlall));

  if (swin->screenatr.current_ppd != nullptr) {
    free_ppd(swin->screenatr.current_ppd);
    swin->screenatr.view_ppd = nullptr;
  }

  swin->screenatr.current_ppd = ppd;
  swin->screenatr.view_ppd = ppd;

  if (ppd->scale <= 0.0) {
    ppd->scale = 1.0;
  }

  fitViewToPpd(&swin->screenatr, ppd);

  swin->screenatr.rad_sph = 0.003;
  swin->screenatr.rad_cyl = 0.0015;
  swin->screenatr.rad_sph *= ppd->scale;
  swin->screenatr.rad_cyl *= ppd->scale;

  set_subdiv_boundary(ppd);
}

bool loadPpdIntoSwin(const QString &path)
{
  if (swin == nullptr) {
    return false;
  }
  QByteArray encoded = QFile::encodeName(path);
  std::vector<char> fname(encoded.begin(), encoded.end());
  fname.push_back('\0');

  Sppd *ppd = open_ppd(fname.data());
  if (ppd == nullptr) {
    return false;
  }
  attachLoadedPpd(ppd);
  return true;
}

bool loadSmfIntoSwin(const QString &path)
{
  if (swin == nullptr) {
    return false;
  }
  QByteArray encoded = QFile::encodeName(path);
  std::vector<char> fname(encoded.begin(), encoded.end());
  fname.push_back('\0');

  Sppd *ppd = open_smf(fname.data());
  if (ppd == nullptr) {
    return false;
  }
  attachLoadedPpd(ppd);
  return true;
}

void savePpdWithDialog(QWidget *parent)
{
  if (swin == nullptr || swin->screenatr.current_ppd == nullptr) {
    QMessageBox::information(parent, QStringLiteral("MeshToSS"),
                             QStringLiteral("No mesh is loaded."));
    return;
  }
  Sppd *ppd = swin->screenatr.current_ppd;
  QString suggest = QString::fromLocal8Bit(ppd->filename);
  if (suggest.isEmpty()) {
    suggest = QStringLiteral("untitled.ppd");
  }
  QString outPath = QFileDialog::getSaveFileName(
      parent,
      QStringLiteral("Save PPD"),
      fileDialogStartPath(suggest),
      QStringLiteral("PPD files (*.ppd);;All files (*)"));
  if (outPath.isEmpty()) {
    return;
  }
  if (!outPath.endsWith(QStringLiteral(".ppd"), Qt::CaseInsensitive)) {
    outPath.append(QStringLiteral(".ppd"));
  }

  QByteArray encoded = QFile::encodeName(outPath);
  std::vector<char> buf(encoded.begin(), encoded.end());
  buf.push_back('\0');
  write_ppd_file(buf.data(), ppd);
  std::strncpy(ppd->filename, buf.data(), BUFSIZ - 1);
  ppd->filename[BUFSIZ - 1] = '\0';
}

void saveSmfWithDialog(QWidget *parent, bool useViewPpd)
{
  Sppd *ppd = nullptr;
  if (useViewPpd) {
    if (swin == nullptr || swin->screenatr.view_ppd == nullptr) {
      QMessageBox::information(parent, QStringLiteral("MeshToSS"),
                               QStringLiteral("No view mesh is loaded."));
      return;
    }
    ppd = swin->screenatr.view_ppd;
  } else {
    if (swin == nullptr || swin->screenatr.current_ppd == nullptr) {
      QMessageBox::information(parent, QStringLiteral("MeshToSS"),
                               QStringLiteral("No mesh is loaded."));
      return;
    }
    ppd = swin->screenatr.current_ppd;
  }

  QString suggest = QString::fromLocal8Bit(ppd->filename);
  if (suggest.isEmpty()) {
    suggest = QStringLiteral("untitled.smf");
  }
  if (suggest.endsWith(QStringLiteral(".ppd"), Qt::CaseInsensitive)) {
    suggest.chop(4);
    suggest.append(QStringLiteral(".smf"));
  }
  QString outPath = QFileDialog::getSaveFileName(
      parent,
      useViewPpd ? QStringLiteral("Save SMF (view mesh)") : QStringLiteral("Save SMF"),
      fileDialogStartPath(suggest),
      QStringLiteral("SMF files (*.smf);;All files (*)"));
  if (outPath.isEmpty()) {
    return;
  }
  if (!outPath.endsWith(QStringLiteral(".smf"), Qt::CaseInsensitive)) {
    outPath.append(QStringLiteral(".smf"));
  }

  QByteArray encoded = QFile::encodeName(outPath);
  std::vector<char> buf(encoded.begin(), encoded.end());
  buf.push_back('\0');
  write_smf(buf.data(), ppd);
  std::strncpy(ppd->filename, buf.data(), BUFSIZ - 1);
  ppd->filename[BUFSIZ - 1] = '\0';
}

void showMeshInfoDialog(QWidget *parent)
{
  if (swin == nullptr || swin->screenatr.current_ppd == nullptr) {
    QMessageBox::information(parent, QStringLiteral("MeshToSS"),
                             QStringLiteral("No mesh is loaded."));
    return;
  }
  Sppd *ppd = swin->screenatr.current_ppd;
  QDialog dlg(parent);
  dlg.setWindowTitle(QStringLiteral("Mesh information"));
  auto *layout = new QVBoxLayout(&dlg);
  auto *text = new QTextEdit(&dlg);
  text->setReadOnly(true);
  text->setPlainText(
      QStringLiteral("ppd\n\nvertices:\t%1\nfaces:\t%2\nedges:\t%3\n")
          .arg(ppd->vn)
          .arg(ppd->fn)
          .arg(ppd->en));
  layout->addWidget(text);
  auto *closeBtn = new QPushButton(QStringLiteral("Close"), &dlg);
  QObject::connect(closeBtn, &QPushButton::clicked, &dlg, &QDialog::accept);
  layout->addWidget(closeBtn);
  dlg.resize(320, 200);
  dlg.exec();
}

class MeshToSSGLView final : public QOpenGLWidget {
public:
  MeshToSSGLView()
  {
    setFocusPolicy(Qt::StrongFocus);
  }

  bool loadPpdFile(const QString &path)
  {
    if (!loadPpdIntoSwin(path)) {
      return false;
    }
    update();
    return true;
  }

  bool loadSmfFile(const QString &path)
  {
    if (!loadSmfIntoSwin(path)) {
      return false;
    }
    update();
    return true;
  }

  void saveVwToPath(const QString &path)
  {
    QByteArray encoded = QFile::encodeName(path);
    std::vector<char> buf(encoded.begin(), encoded.end());
    buf.push_back('\0');
    makeCurrent();
    write_vw_file(buf.data());
    doneCurrent();
  }

  ~MeshToSSGLView() override
  {
    makeCurrent();
    if (swin != nullptr) {
      free_swin(swin);
      swin = nullptr;
    }
    doneCurrent();
  }

protected:
  void initializeGL() override
  {
    swin = create_swin();
    installEmptyBitmapFontLists(&swin->screenatr);
    swin->dis3d.coaxis = TRUE;
    swin->screenatr.bgrgb[0] = 1.0f;
    swin->screenatr.bgrgb[1] = 1.0f;
    swin->screenatr.bgrgb[2] = 1.0f;
    init_gl3d();
  }

  void resizeGL(int w, int h) override
  {
    if (swin == nullptr) {
      return;
    }
    syncScreenSize(&swin->screenatr, w, h);
  }

  void paintGL() override
  {
    if (swin == nullptr) {
      return;
    }
    redraw();
  }

  void mousePressEvent(QMouseEvent *e) override
  {
    if (swin == nullptr) {
      QOpenGLWidget::mousePressEvent(e);
      return;
    }
    if (e->button() == Qt::LeftButton) {
      scrn_x_ = static_cast<int>(e->position().x());
      scrn_y_ = static_cast<int>(e->position().y());
      const Qt::KeyboardModifiers m = e->modifiers();
      if ((m & Qt::ShiftModifier) != 0 && (m & Qt::ControlModifier) != 0) {
        moving_ = true;
        rotating_ = false;
        scaling_ = false;
        e->accept();
        return;
      }
      if ((m & Qt::ShiftModifier) != 0) {
        rotating_ = true;
        moving_ = false;
        scaling_ = false;
        e->accept();
        return;
      }
      if ((m & Qt::ControlModifier) != 0) {
        scaling_ = true;
        moving_ = false;
        rotating_ = false;
        e->accept();
        return;
      }
      moving_ = false;
      rotating_ = false;
      scaling_ = false;
    }
    QOpenGLWidget::mousePressEvent(e);
  }

  void mouseReleaseEvent(QMouseEvent *e) override
  {
    if (e->button() == Qt::LeftButton) {
      moving_ = false;
      rotating_ = false;
      scaling_ = false;
    }
    QOpenGLWidget::mouseReleaseEvent(e);
  }

  void mouseMoveEvent(QMouseEvent *e) override
  {
    if (swin == nullptr || (e->buttons() & Qt::LeftButton) == 0) {
      QOpenGLWidget::mouseMoveEvent(e);
      return;
    }

    const int x = static_cast<int>(e->position().x());
    const int y = static_cast<int>(e->position().y());
    const Qt::KeyboardModifiers m = QApplication::keyboardModifiers();

    if (rotating_ && (m & Qt::ShiftModifier) != 0) {
      const int dx = x - scrn_x_;
      const int dy = y - scrn_y_;
      update_screen3d_rotate(&(swin->screenatr), dx, dy);
      scrn_x_ = x;
      scrn_y_ = y;
      update();
      e->accept();
      return;
    }
    if (moving_ && (m & Qt::ShiftModifier) != 0 && (m & Qt::ControlModifier) != 0) {
      const int dx = x - scrn_x_;
      const int dy = y - scrn_y_;
      update_screen3d_translate(&(swin->screenatr), dx, dy);
      scrn_x_ = x;
      scrn_y_ = y;
      update();
      e->accept();
      return;
    }
    if (scaling_ && (m & Qt::ControlModifier) != 0) {
      const int dx = x - scrn_x_;
      const int dy = y - scrn_y_;
      update_screen3d_zoom(&(swin->screenatr), dx, dy);
      scrn_x_ = x;
      scrn_y_ = y;
      update();
      e->accept();
      return;
    }

    QOpenGLWidget::mouseMoveEvent(e);
  }

  void wheelEvent(QWheelEvent *e) override
  {
    if (swin == nullptr) {
      QOpenGLWidget::wheelEvent(e);
      return;
    }
    int delta = 0;
    if (!e->pixelDelta().isNull()) {
      delta = e->pixelDelta().y();
    } else {
      delta = e->angleDelta().y() / 8;
    }
    if (delta == 0) {
      QOpenGLWidget::wheelEvent(e);
      return;
    }
    update_screen3d_zoom(&(swin->screenatr), delta, 0);
    update();
    e->accept();
  }

private:
  int scrn_x_{0};
  int scrn_y_{0};
  bool moving_{false};
  bool rotating_{false};
  bool scaling_{false};
};

void openVwWithDialog(QWidget *parent, QWidget *glWidget)
{
  if (swin == nullptr) {
    return;
  }
  const QString fn = QFileDialog::getOpenFileName(
      parent,
      QStringLiteral("Open view settings"),
      fileDialogStartPath(),
      QStringLiteral("View files (*.vw);;All files (*)"));
  if (fn.isEmpty()) {
    return;
  }
  QByteArray encoded = QFile::encodeName(fn);
  std::vector<char> buf(encoded.begin(), encoded.end());
  buf.push_back('\0');
  double mmat[16];
  double pmat[16];
  open_vw(buf.data(), mmat, pmat);
  glWidget->update();
}

void showMaterialDialog(QWidget *parent, QWidget *glWidget)
{
  if (swin == nullptr || swin->screenatr.current_ppd == nullptr) {
    QMessageBox::information(parent, QStringLiteral("MeshToSS"),
                             QStringLiteral("No mesh is loaded."));
    return;
  }
  Sppd *ppd = swin->screenatr.current_ppd;

  QDialog dlg(parent);
  dlg.setWindowTitle(QStringLiteral("Material"));
  auto *layout = new QVBoxLayout(&dlg);
  auto *applyBox = new QCheckBox(QStringLiteral("Apply to mesh on double-click"), &dlg);
  applyBox->setChecked(true);
  layout->addWidget(applyBox);

  auto *list = new QListWidget(&dlg);
  for (int i = 0; i < MTLNUM; ++i) {
    list->addItem(QString::fromUtf8(mtltxt[i]));
  }
  list->setMinimumHeight(280);
  layout->addWidget(list);

  QObject::connect(list, &QListWidget::itemDoubleClicked, [ppd, applyBox, glWidget](QListWidgetItem *item) {
    if (!applyBox->isChecked() || item == nullptr) {
      return;
    }
    const int sel = item->listWidget()->row(item);
    if (sel < 0 || sel >= MTLNUM) {
      return;
    }
    copy_material(&(ppd->matl), sel, static_cast<float *>(mtlall));
    glWidget->update();
  });

  auto *closeBtn = new QPushButton(QStringLiteral("Close"), &dlg);
  QObject::connect(closeBtn, &QPushButton::clicked, &dlg, &QDialog::accept);
  layout->addWidget(closeBtn);

  dlg.resize(360, 420);
  dlg.exec();
}

void pickBackgroundColor(QWidget *parent, QWidget *glWidget)
{
  if (swin == nullptr) {
    return;
  }
  ScreenAtr *s = &swin->screenatr;
  QColor initial = QColor::fromRgbF(
      static_cast<double>(s->bgrgb[0]),
      static_cast<double>(s->bgrgb[1]),
      static_cast<double>(s->bgrgb[2]));
  const QColor c = QColorDialog::getColor(initial, parent, QStringLiteral("Background color"));
  if (!c.isValid()) {
    return;
  }
  s->bgrgb[0] = static_cast<float>(c.redF());
  s->bgrgb[1] = static_cast<float>(c.greenF());
  s->bgrgb[2] = static_cast<float>(c.blueF());
  glWidget->update();
}

void saveVwWithDialog(QWidget *parent, MeshToSSGLView *view)
{
  if (swin == nullptr) {
    return;
  }
  const QString outPath = QFileDialog::getSaveFileName(
      parent,
      QStringLiteral("Save view settings"),
      fileDialogStartPath(QStringLiteral("untitled.vw")),
      QStringLiteral("View files (*.vw);;All files (*)"));
  if (outPath.isEmpty()) {
    return;
  }
  QString path = outPath;
  if (!path.endsWith(QStringLiteral(".vw"), Qt::CaseInsensitive)) {
    path.append(QStringLiteral(".vw"));
  }
  view->saveVwToPath(path);
}

/** One level coarser: move view to parent mesh (MFC Edit → Subdivision → Up). */
bool meshSubdivisionLevelUp(QWidget *glWidget)
{
  if (swin == nullptr || swin->screenatr.view_ppd == nullptr) {
    return false;
  }
  Sppd *ppd = swin->screenatr.view_ppd;
  if (ppd->parent == nullptr) {
    return false;
  }
  swin->screenatr.view_ppd = ppd->parent;
  if (glWidget != nullptr) {
    glWidget->update();
  }
  return true;
}

/** One level finer: Loop subdiv child, max depth 3 (MFC Edit → Subdivision → Down). */
bool meshSubdivisionLevelDown(QWidget *glWidget)
{
  if (swin == nullptr || swin->screenatr.view_ppd == nullptr) {
    return false;
  }
  Sppd *ppd = swin->screenatr.view_ppd;
  if (ppd->sub_level >= 3) {
    return false;
  }
  Sppd *child = ppd->child;
  if (child == nullptr) {
    child = ppdsubdiv(ppd, SUBDIV_LOOP);
    child->file_type = FILE_EVRML_POLY;
    copy_material(&(child->matl), 0, static_cast<float *>(mtlall));
  }
  swin->screenatr.view_ppd = child;
  if (glWidget != nullptr) {
    glWidget->update();
  }
  return true;
}

void showConvertSubdivDialog(QWidget *parent, QWidget *glWidget)
{
  if (swin == nullptr || swin->screenatr.current_ppd == nullptr) {
    QMessageBox::information(parent, QStringLiteral("MeshToSS"),
                             QStringLiteral("No mesh is loaded."));
    return;
  }

  QDialog dlg(parent);
  dlg.setWindowTitle(QStringLiteral("Convert Subdivision Surfaces"));
  auto *root = new QVBoxLayout(&dlg);

  swin->conv_type = CONV_NONE;

  auto *radioV = new QRadioButton(QStringLiteral("Number of vertices"), &dlg);
  auto *radioD = new QRadioButton(QStringLiteral("L2-norm distance (%)"), &dlg);
  root->addWidget(radioV);
  root->addWidget(radioD);

  QObject::connect(radioV, &QAbstractButton::toggled, [](bool on) {
    if (on && swin != nullptr) {
      swin->conv_type = CONV_NUMV;
    }
  });
  QObject::connect(radioD, &QAbstractButton::toggled, [](bool on) {
    if (on && swin != nullptr) {
      swin->conv_type = CONV_DIS;
    }
  });

  auto *vBox = new QGroupBox(QStringLiteral("Target vertex count"), &dlg);
  auto *vLay = new QHBoxLayout(vBox);
  vLay->addWidget(new QLabel(QStringLiteral("Current:"), vBox));
  auto *curV = new QLineEdit(vBox);
  curV->setReadOnly(true);
  vLay->addWidget(curV);
  vLay->addWidget(new QLabel(QStringLiteral("Simplified:"), vBox));
  auto *simpV = new QLineEdit(vBox);
  vLay->addWidget(simpV);
  auto *vBtnRow = new QHBoxLayout();
  auto *okV = new QPushButton(QStringLiteral("OK"), &dlg);
  auto *cancelV = new QPushButton(QStringLiteral("Cancel"), &dlg);
  vBtnRow->addWidget(okV);
  vBtnRow->addWidget(cancelV);
  root->addWidget(vBox);
  root->addLayout(vBtnRow);

  auto *dBox = new QGroupBox(QStringLiteral("L2-norm (%)"), &dlg);
  auto *dLay = new QHBoxLayout(dBox);
  dLay->addWidget(new QLabel(QStringLiteral("Current:"), dBox));
  auto *curD = new QLineEdit(dBox);
  curD->setReadOnly(true);
  dLay->addWidget(curD);
  dLay->addWidget(new QLabel(QStringLiteral("Target:"), dBox));
  auto *simpD = new QLineEdit(dBox);
  dLay->addWidget(simpD);
  auto *dBtnRow = new QHBoxLayout();
  auto *okD = new QPushButton(QStringLiteral("OK"), &dlg);
  auto *cancelD = new QPushButton(QStringLiteral("Cancel"), &dlg);
  dBtnRow->addWidget(okD);
  dBtnRow->addWidget(cancelD);
  root->addWidget(dBox);
  root->addLayout(dBtnRow);

  auto *resetBtn = new QPushButton(QStringLiteral("Reset"), &dlg);
  auto *closeBtn = new QPushButton(QStringLiteral("Close"), &dlg);
  auto *bottomRow = new QHBoxLayout();
  bottomRow->addWidget(resetBtn);
  bottomRow->addStretch();
  bottomRow->addWidget(closeBtn);
  root->addLayout(bottomRow);

  const auto refreshFields = [&]() {
    if (swin == nullptr) {
      return;
    }
    Sppd *ref = swin->screenatr.org_ppd != nullptr ? swin->screenatr.org_ppd
                                                    : swin->screenatr.current_ppd;
    curV->setText(QString::number(ref->vn));
    simpV->setText(QString::number(ref->vn));
    curD->setText(QString::number(ref->dis, 'g', 8));
    simpD->setText(QString::number(ref->dis, 'g', 8));
  };

  refreshFields();

  QObject::connect(okV, &QPushButton::clicked, &dlg, [&]() {
    if (swin == nullptr || swin->conv_type != CONV_NUMV) {
      return;
    }
    bool ok = false;
    const int nvertex = simpV->text().trimmed().toInt(&ok);
    if (!ok || nvertex < 0) {
      QMessageBox::warning(&dlg, QStringLiteral("MeshToSS"),
                           QStringLiteral("Enter a valid vertex count."));
      return;
    }

    params_init(&params);
    params.vnum = nvertex;
    params.subdivision = EVAL_2SUB;

    Sppd *ppd = swin->screenatr.current_ppd;
    swin->screenatr.org_ppd = copy_ppd(ppd);
    copy_material(&(swin->screenatr.org_ppd->matl), 0, static_cast<float *>(mtlall));

    simpmesh(ppd, nvertex);

    QMessageBox::information(&dlg, QStringLiteral("MeshToSS"),
                             conversionFinishedMessage());
    if (swin->screenatr.org_ppd != nullptr) {
      curV->setText(QString::number(swin->screenatr.org_ppd->vn));
      curD->setText(QString::number(swin->screenatr.org_ppd->dis, 'g', 8));
    }
    simpD->setText(QString::number(ppd->err, 'g', 8));
    ppd->file_type = FILE_EVRML_SUBDIV;
    set_subdiv_boundary(ppd);
    glWidget->update();
  });

  QObject::connect(cancelV, &QPushButton::clicked, &dlg, [&]() {
    if (swin == nullptr || swin->conv_type != CONV_NUMV) {
      return;
    }
    simpV->clear();
  });

  QObject::connect(okD, &QPushButton::clicked, &dlg, [&]() {
    if (swin == nullptr || swin->conv_type != CONV_DIS) {
      return;
    }
    bool ok = false;
    const double l2 = simpD->text().trimmed().toDouble(&ok);
    if (!ok) {
      QMessageBox::warning(&dlg, QStringLiteral("MeshToSS"),
                           QStringLiteral("Enter a valid L2-norm value."));
      return;
    }

    params_init(&params);
    params.qemmode = MODE_VQEM;
    params.subdivision = EVAL_2SUB;

    Sppd *ppd = swin->screenatr.current_ppd;
    swin->screenatr.org_ppd = copy_ppd(ppd);
    copy_material(&(swin->screenatr.org_ppd->matl), 0, static_cast<float *>(mtlall));

    simpmesh_l2norm(ppd, l2);

    QMessageBox::information(&dlg, QStringLiteral("MeshToSS"),
                             conversionFinishedMessage());
    if (swin->screenatr.org_ppd != nullptr) {
      curV->setText(QString::number(swin->screenatr.org_ppd->vn));
      curD->setText(QString::number(swin->screenatr.org_ppd->dis, 'g', 8));
    }
    simpV->setText(QString::number(ppd->vn));
    ppd->file_type = FILE_EVRML_SUBDIV;
    set_subdiv_boundary(ppd);
    glWidget->update();
  });

  QObject::connect(cancelD, &QPushButton::clicked, &dlg, [&]() {
    if (swin == nullptr || swin->conv_type != CONV_DIS) {
      return;
    }
    simpD->clear();
  });

  QObject::connect(resetBtn, &QPushButton::clicked, &dlg, [&]() {
    if (swin == nullptr || swin->screenatr.org_ppd == nullptr) {
      return;
    }
    free_ppd(swin->screenatr.current_ppd);
    swin->screenatr.current_ppd = swin->screenatr.org_ppd;
    swin->screenatr.view_ppd = swin->screenatr.org_ppd;
    swin->screenatr.org_ppd = nullptr;
    refreshFields();
    glWidget->update();
  });

  QObject::connect(closeBtn, &QPushButton::clicked, &dlg, &QDialog::accept);

  dlg.resize(420, 320);
  dlg.exec();
}

} // namespace

int main(int argc, char *argv[])
{
  initFileDialogBaseDirEarly();
  QApplication app(argc, argv);
  QApplication::setApplicationName(QStringLiteral("MeshToSS"));
  QApplication::setOrganizationName(QStringLiteral("MeshToSS"));

  QSurfaceFormat fmt;
  fmt.setDepthBufferSize(24);
  fmt.setStencilBufferSize(8);
  fmt.setProfile(QSurfaceFormat::CompatibilityProfile);
  QSurfaceFormat::setDefaultFormat(fmt);

  QMainWindow window;
#ifdef __APPLE__
  window.menuBar()->setNativeMenuBar(false);
#endif
  auto *view = new MeshToSSGLView;
  window.setCentralWidget(view);
  window.resize(900, 650);
  window.setWindowTitle(QStringLiteral("MeshToSS (Qt6)"));

  auto *hintLabel = new QLabel(
      QStringLiteral("Shift+drag: rotate | Shift+Ctrl+drag: pan | Ctrl+drag: zoom | Wheel: zoom | "
                     "Ctrl+E: convert subdiv | M/N: subdiv up/down"));
  QFont hintFont = hintLabel->font();
  hintFont.setPointSize(hintFont.pointSize() > 0 ? hintFont.pointSize() - 1 : 10);
  hintLabel->setFont(hintFont);
  hintLabel->setStyleSheet(QStringLiteral("color: palette(mid);"));
  window.statusBar()->addPermanentWidget(hintLabel);

  QMenu *fileMenu = window.menuBar()->addMenu(QStringLiteral("File"));

  auto *openPpdAct = new QAction(QStringLiteral("Open PPD…"), &window);
  openPpdAct->setShortcut(QKeySequence::Open);
  QObject::connect(openPpdAct, &QAction::triggered, &window, [&window, view]() {
    const QString fn = QFileDialog::getOpenFileName(
        &window,
        QStringLiteral("Open PPD"),
        fileDialogStartPath(),
        QStringLiteral("PPD files (*.ppd);;All files (*)"));
    if (fn.isEmpty()) {
      return;
    }
    if (!view->loadPpdFile(fn)) {
      QMessageBox::warning(&window, QStringLiteral("MeshToSS"),
                           QStringLiteral("Could not open the selected file."));
    }
  });
  fileMenu->addAction(openPpdAct);

  auto *openSmfAct = new QAction(QStringLiteral("Open SMF…"), &window);
  /* macOS: Qt::CTRL maps to Command, so this appears as ⌘F in the menu. */
  openSmfAct->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_F));
  openSmfAct->setShortcutContext(Qt::ApplicationShortcut);
  QObject::connect(openSmfAct, &QAction::triggered, &window, [&window, view]() {
    const QString fn = QFileDialog::getOpenFileName(
        &window,
        QStringLiteral("Open SMF"),
        fileDialogStartPath(),
        QStringLiteral("SMF files (*.smf);;All files (*)"));
    if (fn.isEmpty()) {
      return;
    }
    if (!view->loadSmfFile(fn)) {
      QMessageBox::warning(&window, QStringLiteral("MeshToSS"),
                           QStringLiteral("Could not open the selected file."));
    }
  });
  fileMenu->addAction(openSmfAct);

  fileMenu->addSeparator();

  auto *savePpdAct = new QAction(QStringLiteral("Save PPD…"), &window);
  savePpdAct->setShortcut(QKeySequence::Save);
  QObject::connect(savePpdAct, &QAction::triggered, &window, [&window]() {
    savePpdWithDialog(&window);
  });
  fileMenu->addAction(savePpdAct);

  auto *saveSmfAct = new QAction(QStringLiteral("Save SMF…"), &window);
  QObject::connect(saveSmfAct, &QAction::triggered, &window, [&window]() {
    saveSmfWithDialog(&window, false);
  });
  fileMenu->addAction(saveSmfAct);

  auto *saveSmfViewAct = new QAction(QStringLiteral("Save SMF (view mesh)…"), &window);
  QObject::connect(saveSmfViewAct, &QAction::triggered, &window, [&window]() {
    saveSmfWithDialog(&window, true);
  });
  fileMenu->addAction(saveSmfViewAct);

  fileMenu->addSeparator();

  auto *openVwAct = new QAction(QStringLiteral("Open view (.vw)…"), &window);
  QObject::connect(openVwAct, &QAction::triggered, &window, [&window, view]() {
    openVwWithDialog(&window, static_cast<QWidget *>(view));
  });
  fileMenu->addAction(openVwAct);

  auto *saveVwAct = new QAction(QStringLiteral("Save view (.vw)…"), &window);
  QObject::connect(saveVwAct, &QAction::triggered, &window, [&window, view]() {
    saveVwWithDialog(&window, view);
  });
  fileMenu->addAction(saveVwAct);

  fileMenu->addSeparator();
  auto *quitAct = new QAction(QStringLiteral("Quit"), &window);
  quitAct->setShortcut(QKeySequence::Quit);
  QObject::connect(quitAct, &QAction::triggered, &app, &QCoreApplication::quit);
  fileMenu->addAction(quitAct);

  QMenu *editMenu = window.menuBar()->addMenu(QStringLiteral("Edit"));
  auto *convertAct = new QAction(QStringLiteral("Convert to subdivision surface…"), &window);
  /* macOS: Qt::CTRL maps to Command, so the menu shows ⌘E (same as Open SMF). */
  convertAct->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_E));
  convertAct->setShortcutContext(Qt::ApplicationShortcut);
  QObject::connect(convertAct, &QAction::triggered, &window, [&window, view]() {
    showConvertSubdivDialog(&window, static_cast<QWidget *>(view));
  });
  editMenu->addAction(convertAct);

  QMenu *subdivMenu = editMenu->addMenu(QStringLiteral("Subdivision"));
  auto *subdivUpAct = new QAction(QStringLiteral("Up"), &window);
  subdivUpAct->setToolTip(QStringLiteral("One level coarser (parent mesh)"));
  subdivUpAct->setShortcut(Qt::Key_M);
  subdivUpAct->setShortcutContext(Qt::ApplicationShortcut);
  QObject::connect(subdivUpAct, &QAction::triggered, &window, [&window, view]() {
    if (swin == nullptr || swin->screenatr.view_ppd == nullptr) {
      QMessageBox::information(&window, QStringLiteral("MeshToSS"),
                               QStringLiteral("No mesh is loaded."));
      return;
    }
    meshSubdivisionLevelUp(static_cast<QWidget *>(view));
  });
  subdivMenu->addAction(subdivUpAct);

  auto *subdivDownAct = new QAction(QStringLiteral("Down"), &window);
  subdivDownAct->setToolTip(QStringLiteral("One level finer (Loop subdivision, max 3 levels)"));
  subdivDownAct->setShortcut(Qt::Key_N);
  subdivDownAct->setShortcutContext(Qt::ApplicationShortcut);
  QObject::connect(subdivDownAct, &QAction::triggered, &window, [&window, view]() {
    if (swin == nullptr || swin->screenatr.view_ppd == nullptr) {
      QMessageBox::information(&window, QStringLiteral("MeshToSS"),
                               QStringLiteral("No mesh is loaded."));
      return;
    }
    meshSubdivisionLevelDown(static_cast<QWidget *>(view));
  });
  subdivMenu->addAction(subdivDownAct);

  QMenu *meshMenu = window.menuBar()->addMenu(QStringLiteral("Mesh"));
  auto *meshInfoAct = new QAction(QStringLiteral("Mesh information…"), &window);
  QObject::connect(meshInfoAct, &QAction::triggered, &window, [&window]() {
    showMeshInfoDialog(&window);
  });
  meshMenu->addAction(meshInfoAct);

  auto *materialAct = new QAction(QStringLiteral("Material…"), &window);
  QObject::connect(materialAct, &QAction::triggered, &window, [&window, view]() {
    showMaterialDialog(&window, static_cast<QWidget *>(view));
  });
  meshMenu->addAction(materialAct);

  QMenu *viewMenu = window.menuBar()->addMenu(QStringLiteral("View"));

  auto *shadingAct = new QAction(QStringLiteral("Shading"), &window);
  shadingAct->setCheckable(true);
  shadingAct->setChecked(true);
  shadingAct->setShortcut(Qt::Key_Z);
  shadingAct->setShortcutContext(Qt::ApplicationShortcut);
  QObject::connect(shadingAct, &QAction::toggled, view, [view](bool checked) {
    if (swin != nullptr) {
      swin->dis3d.shading = checked ? TRUE : FALSE;
      view->update();
    }
  });
  viewMenu->addAction(shadingAct);

  auto *wireAct = new QAction(QStringLiteral("Wireframe"), &window);
  wireAct->setCheckable(true);
  wireAct->setShortcut(Qt::Key_X);
  wireAct->setShortcutContext(Qt::ApplicationShortcut);
  QObject::connect(wireAct, &QAction::toggled, view, [view](bool checked) {
    if (swin != nullptr) {
      swin->dis3d.wire = checked ? TRUE : FALSE;
      view->update();
    }
  });
  viewMenu->addAction(wireAct);

  auto *vertexAct = new QAction(QStringLiteral("Vertices"), &window);
  vertexAct->setCheckable(true);
  vertexAct->setShortcut(Qt::Key_C);
  vertexAct->setShortcutContext(Qt::ApplicationShortcut);
  QObject::connect(vertexAct, &QAction::toggled, view, [view](bool checked) {
    if (swin != nullptr) {
      swin->dis3d.vertex = checked ? TRUE : FALSE;
      view->update();
    }
  });
  viewMenu->addAction(vertexAct);

  viewMenu->addSeparator();

  auto *hiddenAct = new QAction(QStringLiteral("Hidden lines"), &window);
  hiddenAct->setCheckable(true);
  hiddenAct->setChecked(false);
  QObject::connect(hiddenAct, &QAction::toggled, view, [view](bool checked) {
    if (swin != nullptr) {
      swin->dis3d.hidden = checked ? TRUE : FALSE;
      view->update();
    }
  });
  viewMenu->addAction(hiddenAct);

  auto *coaxisAct = new QAction(QStringLiteral("Show axes"), &window);
  coaxisAct->setCheckable(true);
  coaxisAct->setChecked(true);
  QObject::connect(coaxisAct, &QAction::toggled, view, [view](bool checked) {
    if (swin != nullptr) {
      swin->dis3d.coaxis = checked ? TRUE : FALSE;
      view->update();
    }
  });
  viewMenu->addAction(coaxisAct);

  viewMenu->addSeparator();

  auto *light0Act = new QAction(QStringLiteral("Light 0"), &window);
  light0Act->setCheckable(true);
  light0Act->setChecked(true);
  QObject::connect(light0Act, &QAction::toggled, view, [view](bool checked) {
    if (swin != nullptr) {
      swin->dis3d.light0 = checked ? TRUE : FALSE;
      view->update();
    }
  });
  viewMenu->addAction(light0Act);

  auto *light1Act = new QAction(QStringLiteral("Light 1"), &window);
  light1Act->setCheckable(true);
  light1Act->setChecked(true);
  QObject::connect(light1Act, &QAction::toggled, view, [view](bool checked) {
    if (swin != nullptr) {
      swin->dis3d.light1 = checked ? TRUE : FALSE;
      view->update();
    }
  });
  viewMenu->addAction(light1Act);

  auto *light2Act = new QAction(QStringLiteral("Light 2"), &window);
  light2Act->setCheckable(true);
  light2Act->setChecked(false);
  QObject::connect(light2Act, &QAction::toggled, view, [view](bool checked) {
    if (swin != nullptr) {
      swin->dis3d.light2 = checked ? TRUE : FALSE;
      view->update();
    }
  });
  viewMenu->addAction(light2Act);

  auto *light3Act = new QAction(QStringLiteral("Light 3"), &window);
  light3Act->setCheckable(true);
  light3Act->setChecked(false);
  QObject::connect(light3Act, &QAction::toggled, view, [view](bool checked) {
    if (swin != nullptr) {
      swin->dis3d.light3 = checked ? TRUE : FALSE;
      view->update();
    }
  });
  viewMenu->addAction(light3Act);

  viewMenu->addSeparator();

  auto *bgColorAct = new QAction(QStringLiteral("Background color…"), &window);
  QObject::connect(bgColorAct, &QAction::triggered, &window, [&window, view]() {
    pickBackgroundColor(&window, static_cast<QWidget *>(view));
  });
  viewMenu->addAction(bgColorAct);

  window.show();
  return app.exec();
}
