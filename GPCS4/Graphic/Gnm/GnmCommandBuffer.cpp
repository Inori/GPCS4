#include "GnmCommandBuffer.h"
#include "../Gve/GveCommandBuffer.h"


GnmCommandBuffer::GnmCommandBuffer(RcPtr<gve::GveContex>& context):
	m_context(context)
{

}

GnmCommandBuffer::~GnmCommandBuffer()
{
}