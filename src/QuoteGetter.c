#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { /* GENERATE YOURSELF USING `uuidgen` */ 0x46, 0xC8, 0x8D, 0xEC, 0xBB, 0xB7, 0x40, 0xB4, 0xAB, 0xC3, 0x3E, 0x34, 0x5B, 0x87, 0x54, 0xC7 }
PBL_APP_INFO(MY_UUID,
             "QuoteGetter", "rpaspi",
             1, 0, /* App version */
             RESOURCE_ID_IMAGE_MENU_ICON,
             APP_INFO_STANDARD_APP);

#define STOCKSYMBOL1 1
#define STOCKSYMBOL2 2
#define STOCKQUOTE1  3
#define STOCKQUOTE2  4
#define DY 18

Window window;
Layer anzeige_1;
Layer anzeige_2;
Layer anzeige_3;
Layer anzeige_4;
Layer anzeige_5;
Layer anzeige_6;

static char text_1[32];
static char text_2[32];
static char quote_1[32];
static char quote_2[32];

AppSync sync;
uint8_t sync_buffer[512];


void upClicked(ClickRecognizerRef recognizer, Window *window) {
   //... called on single click ...
  (void)recognizer;
   (void)window;

   // Tuplet upDownTuplet[] = {TupletInteger(UPDOWN, (uint8_t) upORdown)};
   // app_sync_set(&sync, upDownTuplet, ARRAY_LENGTH(upDownTuplet));
}

void downClicked(ClickRecognizerRef recognizer, Window *window) {
   //... called on single click ...
  (void)recognizer;
   (void)window;
}

void select_set_handler(ClickRecognizerRef recognizer, Window *window) {
  (void)recognizer;
  (void)window;
  }

void click_config_provider(ClickConfig **config, Window *window) {
  // See ui/click.h for more information and default values.

  // single click / repeat-on-hold config:
  config[BUTTON_ID_UP]->click.handler = (ClickHandler) upClicked;
  config[BUTTON_ID_UP]->click.repeat_interval_ms = 1000; // "hold-to-repeat" gets overridden if there's a long click handler configured!

  config[BUTTON_ID_DOWN]->click.handler = (ClickHandler) downClicked;
  config[BUTTON_ID_DOWN]->click.repeat_interval_ms = 1000; // "hold-to-repeat" gets overridden if there's a long click handler configured!

  config[BUTTON_ID_SELECT]->click.handler = (ClickHandler) select_set_handler;

  (void)window;
}

static void sync_tuple_changed_callback(const uint32_t key, const Tuple* new_tuple, const Tuple* old_tuple, void* context) {
      (void) old_tuple;

      switch (key) {
      case STOCKSYMBOL1:
        strcpy(text_1, new_tuple->value->cstring);
        layer_mark_dirty(&anzeige_1);
        break;
      case STOCKSYMBOL2:
        strcpy(text_2, new_tuple->value->cstring);
        layer_mark_dirty(&anzeige_2);
        break;
      case STOCKQUOTE1:
        strcpy(quote_1, new_tuple->value->cstring);
        layer_mark_dirty(&anzeige_3);
        break;
      case STOCKQUOTE2:
        strcpy(quote_2, new_tuple->value->cstring);
        layer_mark_dirty(&anzeige_4);
        break;
      }
}

static void sync_error_callback(DictionaryResult dict_error, AppMessageResult app_message_error, void *context) {
      (void) dict_error;
      (void) app_message_error;
      (void) context;
}

void update_anzeige_1_callback(Layer *me, GContext* ctx) {
  (void)me;
  
  graphics_context_set_text_color(ctx, GColorBlack);
  graphics_text_draw(ctx,
		     text_1,
		     fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD),
		     GRect(0,0,55,28),
		     GTextOverflowModeWordWrap,
		     GTextAlignmentLeft,
		     NULL);
}

void update_anzeige_2_callback(Layer *me, GContext* ctx) {
  (void)me;
  
  graphics_context_set_text_color(ctx, GColorBlack);
  graphics_text_draw(ctx,
		     text_2,
		     fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD),
		     GRect(0,0,55,28),
		     GTextOverflowModeWordWrap,
		     GTextAlignmentLeft,
		     NULL);
}

void update_anzeige_3_callback(Layer *me, GContext* ctx) {
  (void)me;
  
  graphics_context_set_text_color(ctx, GColorBlack);
  graphics_text_draw(ctx,
		     quote_1,
		     fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD),
		     GRect(0,0,95,28),
		     GTextOverflowModeWordWrap,
		     GTextAlignmentLeft,
		     NULL);
}

void update_anzeige_4_callback(Layer *me, GContext* ctx) {
  (void)me;
  
  graphics_context_set_text_color(ctx, GColorBlack);
  graphics_text_draw(ctx,
		     quote_2,
		     fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD),
		     GRect(0,0,95,28),
		     GTextOverflowModeWordWrap,
		     GTextAlignmentLeft,
		     NULL);
}

void update_anzeige_5_callback(Layer *me, GContext* ctx) {
  (void)me;
  
  graphics_context_set_text_color(ctx, GColorBlack);
  graphics_text_draw(ctx,
		     "Hallo 3",
		     fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD),
		     GRect(0,0,95,28),
		     GTextOverflowModeWordWrap,
		     GTextAlignmentLeft,
		     NULL);
}

void update_anzeige_6_callback(Layer *me, GContext* ctx) {
  (void)me;
  
  graphics_context_set_text_color(ctx, GColorBlack);
  graphics_text_draw(ctx,
		     "Hallo 4",
		     fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD),
		     GRect(0,0,95,28),
		     GTextOverflowModeWordWrap,
		     GTextAlignmentLeft,
		     NULL);
}

void handle_init(AppContextRef ctx) {
  (void)ctx;
  
  window_init(&window, "Window Name");
  window_stack_push(&window, true /* Animated */);

  layer_init(&anzeige_1, window.layer.frame);
  anzeige_1.update_proc = update_anzeige_1_callback;
  layer_set_frame(&anzeige_1, GRect(2,0*DY,55,28));
  layer_add_child(&window.layer, &anzeige_1);
  layer_mark_dirty(&anzeige_1);
  
  layer_init(&anzeige_2, window.layer.frame);
  anzeige_2.update_proc = update_anzeige_2_callback;
  layer_set_frame(&anzeige_2, GRect(2,1*DY,55,28));
  layer_add_child(&window.layer, &anzeige_2);
  layer_mark_dirty(&anzeige_2);
  
  layer_init(&anzeige_3, window.layer.frame);
  anzeige_3.update_proc = update_anzeige_3_callback;
  layer_set_frame(&anzeige_3, GRect(58,0*DY,95,28));
  layer_add_child(&window.layer, &anzeige_3);
  layer_mark_dirty(&anzeige_3);
  
  layer_init(&anzeige_4, window.layer.frame);
  anzeige_4.update_proc = update_anzeige_4_callback;
  layer_set_frame(&anzeige_4, GRect(58,1*DY,95,28));
  layer_add_child(&window.layer, &anzeige_4);
  layer_mark_dirty(&anzeige_4);
  
  layer_init(&anzeige_5, window.layer.frame);
  anzeige_5.update_proc = update_anzeige_5_callback;
  layer_set_frame(&anzeige_5, GRect(2,2*DY,55,28));
  layer_add_child(&window.layer, &anzeige_5);
  layer_mark_dirty(&anzeige_5);
  
  layer_init(&anzeige_6, window.layer.frame);
  anzeige_6.update_proc = update_anzeige_6_callback;
  layer_set_frame(&anzeige_6, GRect(2,3*DY,55,28));
  layer_add_child(&window.layer, &anzeige_6);
  layer_mark_dirty(&anzeige_6);
  
  // Attach our desired button functionality
  window_set_click_config_provider(&window, (ClickConfigProvider) click_config_provider);

  Tuplet initial_values[] = {
    TupletCString(STOCKSYMBOL1, "Hallo 1"),
    TupletCString(STOCKSYMBOL2, "Hallo 2"),
    TupletCString(STOCKQUOTE1, "XXX.XX +X.XX"),
    TupletCString(STOCKQUOTE2, "XXX.XX +X.XX"),
  };
  app_sync_init(&sync, sync_buffer, sizeof(sync_buffer), initial_values, ARRAY_LENGTH(initial_values), sync_tuple_changed_callback, sync_error_callback, NULL);
}

static void app_deinit(AppContextRef ctx) {
  (void)ctx;
  app_sync_deinit(&sync);
}

void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
    .deinit_handler = &app_deinit,
    .messaging_info = {
      .buffer_sizes = {
        .inbound = 256, // inbound buffer size in bytes
        .outbound = 256, // outbound buffer size in bytes
      },
    },
};
  app_event_loop(params, &handlers);
}