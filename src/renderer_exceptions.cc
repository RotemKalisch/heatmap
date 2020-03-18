#include "renderer_exceptions.h"

RendererException::RendererException(std::string error_msg) :
    std::exception(), 
    m_error_msg(std::move(error_msg))
{}

RendererException::~RendererException() {}

const char* RendererException::what() const noexcept {
    return m_error_msg.data();
}
