#pragma once
#include <pebble.h>

typedef Layer DateLayer;

DateLayer *date_layer_create(GRect frame, Layer *parent_layer);
void date_layer_destroy(DateLayer *this);
