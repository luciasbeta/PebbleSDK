#include <pebble.h>

Window *window;
TextLayer *text_layer;
TextLayer *date_layer;
InverterLayer *inverter_layer;

void handle_timechanges(struct tm *tick_time, TimeUnits units_changed) {
  static char time_buffer[10];
  static char date_buffer[10];
  
  strftime(time_buffer, sizeof(time_buffer), "%H %M %S", tick_time);
  text_layer_set_text(text_layer, time_buffer);
  
  strftime(date_buffer, sizeof(date_buffer), "%b %e",tick_time);
  text_layer_set_text(date_layer, date_buffer);
}
void handle_init(void) {
	// Create a window and text layer
	window = window_create();
	text_layer = text_layer_create(GRect(0, 0, 144, 154));
	
	// Set the text, font, and text alignment
	text_layer_set_text(text_layer, "My first watchface!");
	text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
	text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
	
	// Add the text layer to the window
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer));

  // Creating text layer for the date, must dealocate memory when using *_create function
  date_layer = text_layer_create(GRect(0, 112, 144, 56));
  text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);
  text_layer_set_font(date_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(date_layer));
  
  // Inverter layer - invert background color
  inverter_layer = inverter_layer_create(GRect(0, 112, 144, 56));
  layer_add_child(window_get_root_layer(window), inverter_layer_get_layer(inverter_layer));
  
  // The following is so the first text layer message doesn't show when switching to watchface
  time_t now = time(NULL);
  handle_timechanges(localtime(&now), SECOND_UNIT);
  // New - subscribe to time service - notified every second - pull from new handle_timechanges function
  tick_timer_service_subscribe(SECOND_UNIT, handle_timechanges);
  
  // Push the window
	window_stack_push(window, true);
	
	// App Logging!
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Just pushed a window! Declaring victory!");
}

void handle_deinit(void) {
	// Destroy the text layer
	text_layer_destroy(text_layer);
  text_layer_destroy(date_layer);
  inverter_layer_destroy(inverter_layer);
	
	// Destroy the window
	window_destroy(window);
}

int main(void) {
	handle_init();
	app_event_loop();
	handle_deinit();
}
