#include "register_types.h"

#include "audio_stream_flac.h"
#include "core/object/class_db.h"

#include "core/config/engine.h"
#include "resource_importer_flac.h"

void initialize_flac_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	if (Engine::get_singleton()->is_editor_hint()) {
		Ref<ResourceImporterFLAC> flac_import;
		flac_import.instantiate();
		ResourceFormatImporter::get_singleton()->add_importer(flac_import);
	}

	ClassDB::register_class<AudioStreamFLAC>();
}

void uninitialize_flac_module(ModuleInitializationLevel p_level) {
}