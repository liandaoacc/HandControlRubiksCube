#ifndef HANDEVENT_H
#define HANDEVENT_H

#include <QEvent>
#include <QPoint>

enum HandEventTypes
{
	HandFistPress = QEvent::User + 1,
	HandFistRelease = QEvent::User + 2,
	HandFistMove = QEvent::User + 3,
	HandOneFinger = QEvent::User + 4,
	HandTwoFinger = QEvent::User + 5,
	HandThreeFinger = QEvent::User + 6,
	HandPalmPress = QEvent::User + 7,
	HandPalmRelease = QEvent::User + 8,
	HandPalmMove = QEvent::User + 9
};

class HandEvent : public QEvent
{
public:
	HandEvent(HandEventTypes type, const QPoint &pos);
	~HandEvent();
	inline const QPoint &pos() const { return p; }
	inline int x() const { return p.x(); }
	inline int y() const { return p.y(); }

protected:
	QPoint p;
};

#endif // MYEVENT_H
