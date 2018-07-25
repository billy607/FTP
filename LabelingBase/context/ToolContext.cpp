#include "ToolContext.h"
namespace labelingbase {

template <> ToolContext * Context<ToolContext>::instance = nullptr;
ToolContext::ToolContext()
{
    //attachmentLoader = nullptr;
}
}


