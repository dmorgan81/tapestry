#pragma once
#include <pebble.h>

typedef Layer BannerLayer;

BannerLayer *banner_layer_create(GRect frame);
void banner_layer_destroy(BannerLayer *this);
