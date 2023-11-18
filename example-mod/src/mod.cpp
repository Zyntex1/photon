#include "mod.h"

#include "convars/convars.h"
#include "hooks/hooks.h"
#include "huds/hud.h"

c_wormhole_mod mod;

expose_wormhole_mod( c_wormhole_mod, mod );

wh_api::c_shared *wh;

sdk::h_font font;

c_example_hud *ex_hud;
c_example_thud *ex_thud;

bool c_wormhole_mod::load( wh_api::c_shared *wh ) {
	::wh = wh;

	if ( example_mod::hooks::initialize( ) ) {
		convars::initialize( );

		wh->huds->reg( ex_hud = new c_example_hud( ) );
		wh->huds->reg( ex_thud = new c_example_thud( ) );

		wh->portal2->console->msg( "example mod loaded.\n" );

		// post example event
		// this will look like: "example mod:load" for other mods
		wh->events->post( &mod, "load" );

		wh->render->create_font( font, "Tahoma", 12, false, sdk::fontflag_dropshadow );
	}

	return true;
}

void c_wormhole_mod::unload( ) {
	wh->huds->unreg( ex_hud );
	wh->huds->unreg( ex_thud );

	convars::uninitialize( );

	example_mod::hooks::uninitialize( );

	wh->portal2->console->msg( "example mod unloaded.\n" );
}

void c_wormhole_mod::on_event( const char *msg ) {
	if ( !strcmp( msg, "paint" ) ) {
		wh->render->draw_text( 2, 2, font, { 255, 255, 255, 255 }, false, "example mod loaded." );
	}
	if ( !strcmp( msg, "on_screen_size_changed" ) ) {
		wh->render->create_font( font, "Tahoma", 12, false, sdk::fontflag_dropshadow );
	}
}

wh_api::wh_mod_info_t *c_wormhole_mod::get_info( ) {
	wh_api::wh_mod_info_t info;
	info.name = "example mod";
	info.version = "0.0.1";
	return &info;
}

void c_wormhole_mod::paint_menu( ) {
	static bool example_checkbox_val;
	wh->menu->checkbox( example_checkbox_val, "example checkbox" );
	static int example_slider_val;
	wh->menu->slider( example_slider_val, 0, 100, "example slider" );
	static float example_sliderf_val;
	wh->menu->sliderf( example_sliderf_val, 0.f, 10.f, "example sliderf" );
	static int example_combo_val;
	static bool example_combo_open;
	const char *items[] = { "value 1", "value 2", "value 3" };
	wh->menu->combo( example_combo_val, example_combo_open, items, 3, "example combo" );
}