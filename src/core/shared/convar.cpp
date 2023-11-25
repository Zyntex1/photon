#include "wormhole-sdk/convar.h"

#include "wormhole-sdk/wormhole.h"

sdk::con_var *c_convar::create_convar( const char *name, const char *default_value, int flags, const char *help_string, bool has_min, float min, bool has_max, float max, sdk::fn_change_callback_t cbk ) {
	auto cvar = reinterpret_cast<sdk::con_var *>( wh->portal2->mem_alloc->alloc( sizeof( sdk::con_var ) ) );
	memset( cvar, 0, sizeof( sdk::con_var ) );

	static const auto ctor_addr = utils::memory::pattern_scan( os( "vstdlib.dll", "libvstdlib.so" ), os( "55 8B EC F3 0F 10 45 ? 8B 55 14", "55 89 E5 56 0F B6 45 24" ) );

	cvar->vtable = *( void ** ) ( ctor_addr + os( 0x34, 0x31 ) );
	cvar->vtable_convar = *( void ** ) ( ctor_addr + os( 0x3b, 0x38 ) );

	using create_fn_t = void( __rescall * )( void *, const char *, const char *, int, const char *, bool, float, bool, float, sdk::fn_change_callback_t );
	const auto create_fn = utils::memory::read<create_fn_t>( ctor_addr + os( 0x6c, 0x7b ) );
	create_fn( cvar, name, default_value, flags, help_string, has_min, min, has_max, max, cbk );

	convars.insert( std::make_pair( name, cvar ) );

	return cvar;
}
void c_convar::destruct_convar( const char *name ) {
	if ( !convars.count( name ) )
		return;

	const auto &cvar = convars[ name ];

	// call dtors here using virtual fns instead of signatures, couldnt figure it out yet

	convars.erase( name );
}

sdk::con_command *c_convar::create_concmd( const char *name, sdk::fn_command_callback_t cbk, const char *help_string, int flags ) {
	auto concmd = reinterpret_cast<sdk::con_command *>( wh->portal2->mem_alloc->alloc( sizeof( sdk::con_command ) ) );
	memset( concmd, 0, sizeof( sdk::con_command ) );

	concmd->vtable = wh->portal2->cvar->find_command_base( "listdemo" )->vtable;
	concmd->name = name;
	concmd->fn_command_callback = cbk;
	concmd->help_string = help_string;
	concmd->flags = flags;
	concmd->fn_completion_callback = 0;
	concmd->has_completion_callback = false;
	concmd->using_new_command_callback = true;
	concmd->using_command_callback_interface = false;

	wh->portal2->cvar->register_con_command( concmd );

	concmds.insert( std::make_pair( name, concmd ) );

	return concmd;
}
void c_convar::destruct_concmd( const char *name ) {
	if ( !concmds.count( name ) )
		return;

	auto concmd = concmds[ name ];

	wh->portal2->cvar->unregister_con_command( concmd );

	wh->portal2->mem_alloc->free( concmd );

	concmds.erase( name );
}

void c_convar::hook_cmd( const char *name, sdk::fn_command_callback_t detour, sdk::fn_command_callback_t &original ) {
	auto concmd = reinterpret_cast<sdk::con_command *>( wh->portal2->cvar->find_command_base( name ) );

	if ( !concmd )
		return;

	original = concmd->fn_command_callback;
	concmd->fn_command_callback = detour;
}
void c_convar::unhook_cmd( const char *name, sdk::fn_command_callback_t original ) {
	auto concmd = reinterpret_cast<sdk::con_command *>( wh->portal2->cvar->find_command_base( name ) );

	if ( !concmd || !original )
		return;

	concmd->fn_command_callback = original;
}
