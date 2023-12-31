#include "hud.h"

#include <photon.h>

c_example_hud *ex_hud;
c_example_thud *ex_thud;

bool huds::initialize( ) {
	photon->hud->reg( ex_hud = new c_example_hud( ) );
	photon->hud->reg( ex_thud = new c_example_thud( ) );

	return true;
}

void huds::uninitialize( ) {
	photon->hud->unreg( ex_hud );
	photon->hud->unreg( ex_thud );
}


void c_example_hud::paint( ) {
	photon->hud->draw_begin( this );

	photon->hud->draw_filled_rect( 0, 0, 20, 20, sdk::color_t( 255, 0, 0, 255 ) );
	photon->hud->draw_filled_rect( 20, 20, 20, 20, sdk::color_t( 0, 255, 0, 255 ) );
	photon->hud->draw_filled_rect( 40, 40, 20, 20, sdk::color_t( 0, 0, 255, 255 ) );
	photon->hud->draw_filled_rect( 0, 60, 80, 20, sdk::color_t( 255, 255, 255, 255 ) );

	photon->hud->draw_end( );
}
const char *c_example_hud::get_name( ) {
	return "example hud";
}


const char *c_example_thud::get_text( ) {
	return "0";
}
const char *c_example_thud::get_name( ) {
	return "example-thud";
}
