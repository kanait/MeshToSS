//
// light.h
//
// Copyright (c) 1997-1999 Takashi Kanai; All rights reserved. 
//

#ifndef _LIGHT_H
#define _LIGHT_H

#define NUM_LIGHT 8

typedef struct _light {
  float   position[4];
  float   ambient[4];
  float   diffuse[4];
  float   specular[4];
  float const_attenuation;
  float linear_attenuation;
} Light;

// light0
static Light light0[] = {
  100.0f, 100.0f, 100.0f, 1.0f, 
  0.1f, 0.1f, 0.1f, 1.0f,
  0.65f, 0.65f, 0.65f, 1.0f,
  0.8f, 0.8f, 0.8f, 1.0f,
  0.1f,
  0.05f
};
  
// light1
static Light light1[] = {
  -100.0f, 100.0f, 100.0f, 1.0f, 
  0.1f, 0.1f, 0.1f, 1.0f,
  0.65f, 0.65f, 0.65f, 1.0f,
  0.8f, 0.8f, 0.8f, 1.0f,
  0.1f,
  0.05f
};
  
// light2
static Light light2[] = {
  -100.0f, 100.0f, -100.0f, 1.0f, 
  0.1f, 0.1f, 0.1f, 1.0f,
  0.65f, 0.65f, 0.65f, 1.0f,
  0.8f, 0.8f, 0.8f, 1.0f,
  0.1f,
  0.05f
};
  
// light3
static Light light3[] = {
  100.0f, 100.0f, -100.0f, 1.0f, 
  0.1f, 0.1f, 0.1f, 1.0f,
  0.65f, 0.65f, 0.65f, 1.0f,
  0.8f, 0.8f, 0.8f, 1.0f,
  0.1f,
  0.05f
};
  
#endif // _LIGHT_H
