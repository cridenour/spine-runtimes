#include <stdexcept>
#include <spine-sdl2/AtlasAttachmentLoader.h>
#include <spine-sdl2/Atlas.h>
#include <spine-sdl2/RegionAttachment.h>

namespace spine {

AtlasAttachmentLoader::AtlasAttachmentLoader (Atlas *atlas) :
				atlas(atlas) {
}

Attachment* AtlasAttachmentLoader::newAttachment (AttachmentType type, const std::string &name) {
	switch (type) {
	case region: {
		AtlasRegion *region = atlas->findRegion(name);
		if (!region) throw std::runtime_error("Atlas region not found: " + name);
		return new RegionAttachment(region);
	}
	default:
		throw std::runtime_error("Unknown attachment type: " + type);
	}
}

} /* namespace spine */
