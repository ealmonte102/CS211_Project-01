#ifndef DATE_H
#define DATE_H

#include <string>

class Date {
public:
	// Initializes a date to the default value of January 1, 1970
	Date ( );

	//Initalizes a date the the values in the parameters. If the
	//date is not legal, sets the date to one of the legal values.
	Date (int m, int d, int y);

	// Returns the month stored by the class
	std::string getMonth ( ) const;

	// Returns the month stored by the class as a number
	int getMonthNum ( ) const;

	//Returns the day stored by the class
	int getDay ( ) const;

	//Returns the year stored by the class
	int getYear ( ) const;

	// Solicit the date from the user
	friend std::istream& operator>>(std::istream& input, Date& currentDate);

	//Output the date in a nice format
	friend std::ostream& operator<<(std::ostream& output, const Date& currentDate);

private:
	int month, day, year;
	void adjust ( );
};
#endif
