#pragma once

#include <exception>
#include <string>

struct RendererException : std::exception {
public:
    RendererException(std::string error_msg);
    virtual ~RendererException();
    virtual const char* what() const noexcept override;
    
private:
    std::string m_error_msg;
};
