#pragma once
#include <glm/glm.hpp>

namespace Real {

	struct FramebufferSpecification {
		unsigned width, height;
		bool     renderInsideThisBuffer = false;
	};

	struct Framebuffer {
		static Ref<Framebuffer> create(const FramebufferSpecification& spec);

		virtual const FramebufferSpecification& getSpec() const                  = 0;
		virtual void                            bind()                           = 0;
		virtual void                            unbind()                         = 0;
		virtual unsigned                        getColorAttachmentID()           = 0;
		virtual void                            resize(const glm::vec2& newSize) = 0;

		virtual ~Framebuffer() = default;
	};
}