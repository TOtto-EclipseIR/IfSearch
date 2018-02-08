/*! @file eirBase.cpp Definitions for eirBase library
  *
  */
#include "eirRes.h"

eirRes eirRes::instance;

eirRes::eirRes(void)
    : Module(__MODULE__)
{
}
