#ifndef DIED_ACTIONTARGET_H
#define DIED_ACTIONTARGET_H

namespace DiED
{
	class ActionTarget
	{
	public:
		virtual void vHandleInsertText(const Glib::ustring & sString) = 0;
		virtual void vHandlePosition(int iLineRelative, int iCharacterRelative, int iLineAbsolute, int iCharacterAbsolute) = 0;
	};
}

#endif
