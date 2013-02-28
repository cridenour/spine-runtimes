#include <iostream>
#include <SDL.h>
#include <spine-sdl2/util.h>
#include <spine-sdl2/RegionAttachment.h>
#include <spine-sdl2/Atlas.h>
#include <spine-sdl2/Skeleton.h>
#include <spine/Bone.h>
#include <spine/Slot.h>

namespace spine {

RegionAttachment::RegionAttachment (AtlasRegion *region) {
	texture = region->page->texture; // BOZO - Resolve attachment as late as possible?
	int u = region->x;
	int u2 = u + region->width;
	int v = region->y;
	int v2 = v + region->height;
	if (region->rotate) {
		vertices[1].tex_coord.x = u;
		vertices[1].tex_coord.y = v2;
		vertices[2].tex_coord.x = u;
		vertices[2].tex_coord.y = v;
		vertices[3].tex_coord.x = u2;
		vertices[3].tex_coord.y = v;
		vertices[0].tex_coord.x = u2;
		vertices[0].tex_coord.y = v2;
	} else {
		vertices[0].tex_coord.x = u;
		vertices[0].tex_coord.y = v2;
		vertices[1].tex_coord.x = u;
		vertices[1].tex_coord.y = v;
		vertices[2].tex_coord.x = u2;
		vertices[2].tex_coord.y = v;
		vertices[3].tex_coord.x = u2;
		vertices[3].tex_coord.y = v2;
	}
}

void RegionAttachment::draw (Slot *slot) {
	Skeleton* skeleton = (Skeleton*)slot->skeleton;

	Uint8 r = skeleton->r * slot->r * 255;
	Uint8 g = skeleton->g * slot->g * 255;
	Uint8 b = skeleton->b * slot->b * 255;
	Uint8 a = skeleton->a * slot->a * 255;
	vertices[0].color.r = r;
	vertices[0].color.g = g;
	vertices[0].color.b = b;
	vertices[0].color.unused = a;
	vertices[1].color.r = r;
	vertices[1].color.g = g;
	vertices[1].color.b = b;
	vertices[1].color.unused = a;
	vertices[2].color.r = r;
	vertices[2].color.g = g;
	vertices[2].color.b = b;
	vertices[2].color.unused = a;
	vertices[3].color.r = r;
	vertices[3].color.g = g;
	vertices[3].color.b = b;
	vertices[3].color.unused = a;

	updateOffset(); // BOZO - Move to resolve()?
	updateWorldVertices(slot->bone);

	skeleton->texture = texture;
	skeleton->vertexArray.push_back(vertices[0]);
	skeleton->vertexArray.push_back(vertices[1]);
	skeleton->vertexArray.push_back(vertices[2]);
	skeleton->vertexArray.push_back(vertices[3]);
}

void RegionAttachment::updateWorldVertices (spine::Bone *bone) {
	float x = bone->worldX;
	float y = bone->worldY;
	float m00 = bone->m00;
	float m01 = bone->m01;
	float m10 = bone->m10;
	float m11 = bone->m11;
	vertices[0].position.x = offset[0] * m00 + offset[1] * m01 + x;
	vertices[0].position.y = offset[0] * m10 + offset[1] * m11 + y;
	vertices[1].position.x = offset[2] * m00 + offset[3] * m01 + x;
	vertices[1].position.y = offset[2] * m10 + offset[3] * m11 + y;
	vertices[2].position.x = offset[4] * m00 + offset[5] * m01 + x;
	vertices[2].position.y = offset[4] * m10 + offset[5] * m11 + y;
	vertices[3].position.x = offset[6] * m00 + offset[7] * m01 + x;
	vertices[3].position.y = offset[6] * m10 + offset[7] * m11 + y;
}

} /* namespace spine */
