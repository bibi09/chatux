#ifndef __CHATUX_POSITION__
#define __CHATUX_POSITION__

class Position {
	public:
		/** @brief	X axis. */
		short x ;
		/** @brief	Y axis. */
		short y ;

		/** @brief	Create a position. */
		Position(short x = 0, short y = 0) ;

		/** @brief	Destroy a position. */
		virtual ~Position() ;
} ;


typedef Position Dimension ;

#endif
