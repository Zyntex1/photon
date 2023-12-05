#include "photon-sdk/hook.h"

#include "deps/subhook/subhook.h"
#include "photon-sdk/utils.h"

int c_hook::create_hook( void *src, void *dst, void *&hook, void **orig ) {
	hook = subhook_new( src, dst, SUBHOOK_TRAMPOLINE );
	*orig = subhook_get_trampoline( ( subhook_t ) hook );
	utils::console::log( "[ photon ] redirected %p to %p, trampoline set at %p.\n", src, dst, *orig );
	return subhook_install( ( subhook_t ) hook );
}

int c_hook::remove_hook( void *hook ) {
	auto result = subhook_remove( ( subhook_t ) hook );
	subhook_free( ( subhook_t ) hook );
	return result;
}

int c_hook::enable_hook( void *hook ) {
	return subhook_install( ( subhook_t ) hook );
}

int c_hook::disable_hook( void *hook ) {
	return subhook_remove( ( subhook_t ) hook );
}
