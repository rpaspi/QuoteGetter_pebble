#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { /* GENERATE YOURSELF USING `uuidgen` */ 0x46, 0xC8, 0x8D, 0xEC, 0xBB, 0xB7, 0x40, 0xB4, 0xAB, 0xC3, 0x3E, 0x34, 0x5B, 0x87, 0x54, 0xC7 }
PBL_APP_INFO(MY_UUID,
             "QuoteGetter", "rpaspi",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_STANDARD_APP);

#define TEXT1 1
#define TEXT2 2

Window window;
Layer anzeige_1;
Layer anzeige_2;

static char text_1[32];
static char text_2[32];

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
      case UPDOWN:
        break;
      case DISPLAY:
        strncpy(anzeige, new_tuple->value->cstring, 6);
        layer_mark_dirty(&anzeigeText);
        break;
        /*
      case QUOTELAST:
        strncpy(appleData.last, new_tuple->value->cstring, 7);
        layer_mark_dirty(&quoteApple);
        break;
      case QUOTECHANGE:
        strncpy(appleChange, new_tuple->value->cstring, 6);
        layer_mark_dirty(&changeText);
        break;
        */
      }
}

static void sync_error_callback(DictionaryResult dict_error, AppMessageResult app_message_error, void *context) {
      (void) dict_error;
      (void) app_message_error;
      (void) context;
}

void update_anzeigeText_callback(Layer *me, GContext* ctx) {
  (void)me;
  
  graphics_context_set_text_color(ctx, GColorBlack);
  graphics_text_draw(ctx,
		     anzeige,
		     fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD),
		     GRect(0,0,140,24),
		     GTextOverflowModeWordWrap,
		     GTextAlignmentLeft,
		     NULL);
}

void update_changeText_callback(Layer *me, GContext* ctx) {
  (void)me;
  
  graphics_context_set_text_color(ctx, GColorBlack);
  graphics_text_draw(ctx,
		     "+1.23\%",
		     fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD),
		     GRect(0,0,47,28),
		     GTextOverflowModeWordWrap,
		     GTextAlignmentLeft,
		     NULL);
}


void handle_init(AppContextRef ctx) {
  (void)ctx;
  
  window_init(&window, "Window Name");
  window_stack_push(&window, true /* Animated */);

  layer_init(&anzeigeText, window.layer.frame);
  anzeigeText.update_proc = update_anzeigeText_callback;
  layer_set_frame(&anzeigeText, GRect(42,10,55,28));
  layer_add_child(&window.layer, &anzeigeText);
  layer_mark_dirty(&anzeigeText);
  
  text_layer_init(&appleText, GRect(2,10,40,28));
  text_layer_set_font(&appleText, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text(&appleText, appleSymbol);
  layer_add_child(&window.layer, &appleText.layer);

  layer_init(&changeText, window.layer.frame);
  changeText.update_proc = update_changeText_callback;
  layer_set_frame(&changeText, GRect(97,10,47,28));
  layer_add_child(&window.layer, &changeText);
  layer_mark_dirty(&changeText);
  

  // Attach our desired button functionality
  window_set_click_config_provider(&window, (ClickConfigProvider) click_config_provider);

  Tuplet initial_values[] = {
    TupletInteger(UPDOWN, (uint8_t) upORdown),
    TupletCString(DISPLAY, "Hallo"),
    TupletCString(QUOTECHANGE, "+0.0\%"),
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