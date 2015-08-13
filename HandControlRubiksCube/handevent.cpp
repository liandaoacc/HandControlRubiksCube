#include "handevent.h"

HandEvent::HandEvent(HandEventTypes type, const QPoint &pos)
	: QEvent((QEvent::Type)type), p(pos)
{
}

HandEvent::~HandEvent()
{
}