#pragma once
#include <pebble.h>

typedef Layer TapestryLayer;

TapestryLayer *tapestry_layer_create(GRect frame);
void tapestry_layer_destroy(TapestryLayer *this);
