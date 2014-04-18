#include <pebble.h>

static Window *window;	
static TextLayer *text_layer;
static BitmapLayer *turn_layer;
static GBitmap *turn;
static TextLayer *distance_text_layer;
	
// Key values for AppMessage Dictionary
enum {
	DIRECTIONS_KEY = 0,
  DISTANCE_KEY = 1,
  ARROW_KEY = 2
};

static void paint_directions(char* directions, char* distance, int right) {
  Layer *window_layer = window_get_root_layer(window);
  text_layer_set_text(text_layer, directions);
  text_layer_set_text(distance_text_layer, distance);
  if(right==1) {
    turn = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_RIGHT_TURN_WHITE);
    vibes_short_pulse();
  }
  else {
    turn = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_LEFT_TURN_WHITE);
    vibes_double_pulse();
  }
  bitmap_layer_set_bitmap(turn_layer, turn);
  layer_add_child(window_layer, bitmap_layer_get_layer(turn_layer));
}

// Called when a message is received from PebbleKitJS
static void in_received_handler(DictionaryIterator *received, void *context) {
	Tuple *tuple;
	Tuple *tuple1;
	Tuple *tuple2;
	
	tuple = dict_find(received, DIRECTIONS_KEY);
	tuple1 = dict_find(received, DISTANCE_KEY);
	tuple2 = dict_find(received, ARROW_KEY);
	if(tuple) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Received Message: %s", tuple->value->cstring);
    paint_directions(tuple->value->cstring, tuple1->value->cstring, tuple2->value->uint32);
    //text_layer_set_text(text_layer, tuple->value->cstring);
	}}


static void init(void) {
	window = window_create();
  window_set_background_color(window, GColorBlack);
	window_stack_push(window, true);
	
	// Register AppMessage handlers
	app_message_register_inbox_received(in_received_handler); 
		
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  text_layer = text_layer_create((GRect) {.origin = { 0,50}, .size = {bounds.size.w, 50}});
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_text_color(text_layer, GColorWhite);
  text_layer_set_background_color(text_layer, GColorBlack);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
  
  distance_text_layer = text_layer_create((GRect) {.origin = {25,8}, .size ={bounds.size.w, 50}});
  text_layer_set_text_alignment(distance_text_layer, GTextAlignmentCenter);
  text_layer_set_font(distance_text_layer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
  text_layer_set_text_color(distance_text_layer, GColorWhite);
  text_layer_set_background_color(distance_text_layer, GColorBlack);
  layer_add_child(window_layer, text_layer_get_layer(distance_text_layer));
 
  //creating bitmaplayer that holds the turn signal image
  turn = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_LEFT_TURN_WHITE);

  turn_layer = bitmap_layer_create((GRect) {.origin = {5,5}, .size = {48,48} });
  bitmap_layer_set_bitmap(turn_layer, turn);
  bitmap_layer_set_alignment(turn_layer, GAlignTopLeft);
  bitmap_layer_set_background_color(turn_layer, GColorClear);
}

static void deinit(void) {
	app_message_deregister_callbacks();
  text_layer_destroy(text_layer);
	window_destroy(window);
}

int main( void ) {
	init();
	app_event_loop();
	deinit();
}