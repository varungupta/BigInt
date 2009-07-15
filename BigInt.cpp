/**
 * BigInt class stores and manupulates Integers as an ordered array of 
 * groups of digits where a group is considered as an integer. Thi approach 
 * allows faster integer operations as compared to traditional way of treating 
 * large integers as array of chars with each char representing one digit.
 * 
 * Author: Varun Gupta <thevarungupta A T gmail>
 *
 * Caveat: This is a work under progress and therefore you may witness bugs
 *         Please feel free to report any sort of discrepancies.
 * 
 * Last Modified: July 14, 2009
 * 
 * Proposed Enhancements:
 * 	1. Dynamic Bucket Allocation. This will remove the upper limit on number 
 *	   of buckets and thus number of digits.
 *	2. Validation of method parameters
 */

#include <iostream>
#include <string>
#include <cstring>
#include <sstream>

using namespace std;

#define MAX 20					// Max. number of buckets. Default 20 buckets (max 20*9 digits).
#define BUCKET_LIMIT 1000000000			// 999999999 is the highest possible number in a single bucket
#define BUCKET_NUM_DIGITS 9			// Max. number of digits of in one bucket

class BigInt	{
	private:
		long long content[MAX];		// Buckets holding actual number
		short sign;			// -1/0/1; depending on number being negetive, zero r positive respectively
		unsigned short length;		// Number of buckets actually utilized to hold the number

		void addLSBucket(int);		// Add Least Significant Buckets: Internally used in multiplication operation

	public:
		// Constructors
		BigInt ();
		BigInt (long long);
		BigInt (string);
		BigInt (const BigInt &n);

		// Assignment Operators
		BigInt operator = (BigInt);

		// Comparison operators
		bool operator == (BigInt);
		bool operator == (int);		// TODO
		bool operator != (BigInt);
		bool operator != (int);		// TODO
		bool operator < (BigInt);
		bool operator < (int);		// TODO
		bool operator <= (BigInt);
		bool operator <= (int);		// TODO
		bool operator > (BigInt);
		bool operator > (int);		// TODO
		bool operator >= (BigInt);
		bool operator >= (int);		// TODO

		// Arithmetic operators
		BigInt operator + (int);	// TODO
		BigInt operator + (BigInt);
		BigInt operator - (int);	// TODO
		BigInt operator - (BigInt);
		BigInt operator * (int);
		BigInt operator * (BigInt);
		BigInt operator / (int);	// TODO
		BigInt operator / (BigInt);	// TODO

		// Methods
		string toString(void);		// Serializes BigInt object
		void debug(void);
};

BigInt :: BigInt ()	{
	length = 0;
	sign = 0;
	memset(content, 0, sizeof(content));
}

BigInt :: BigInt (long long n)	{
	length = 0;

	if (n == 0)	{
		sign = 0;
	}
	else	{
		sign = (n > 0) ? 1 : -1;
		if (n < 0)	n = 0 - n;		// Removing minus sign
		
		for (int i = 0; n > 0; n /= BUCKET_LIMIT)	{
			length ++;
			content[i ++] = n % BUCKET_LIMIT;
		}
	}
	memset(content + length, 0, (MAX - length) * sizeof(long long));
}

BigInt :: BigInt (string n)	{
	length = 0;
	if (n[0] == '0')	{
		sign = 0;
		memset(content, 0, sizeof(content));
	}
	else	{
		int i, posFirstDigit;
		stringstream s;

		if (n[0] == '-')	{
			sign = -1;
			posFirstDigit = 1;
		}
		else	{
			sign = 1;
			posFirstDigit = 0;
		}
		for (i = n.size() - BUCKET_NUM_DIGITS; i >= posFirstDigit; i -= 9)	{

			s << n.substr(i, BUCKET_NUM_DIGITS);
			s >> content[length ++];
			s.clear();
		}
		if (i < posFirstDigit)	{
			s << n.substr(posFirstDigit, BUCKET_NUM_DIGITS - posFirstDigit + i);
			s >> content[length ++];
		}
		memset(content + length, 0, (MAX - length) * sizeof(long long));
	}
}

BigInt :: BigInt (const BigInt &n)	{
	length = n.length;
	sign = n.sign;
	memcpy(content, n.content, MAX * sizeof(long long));
}

void BigInt :: addLSBucket (int numBucketsToBeAdded)	{

	if (length + numBucketsToBeAdded > MAX)	return;

	for (int i = length + numBucketsToBeAdded -1; i >= 0; i --)	{
		content[i] = (i > numBucketsToBeAdded - 1) ? content[i - numBucketsToBeAdded] : 0;
	}
	length += numBucketsToBeAdded;
}

string BigInt :: toString(void)	{
	if (length == 0)	return "0";

	stringstream s;

	if (sign == -1)	s << "-";

	s << content[length - 1];

	for (int i = length - 2; i >= 0; i --)	{
		if (content[i] < 10)	{
			s << "00000000" << content[i];
		}
		else if (content[i] < 100)	{
			s << "0000000" << content[i];
		}
		else if (content[i] < 1000)	{
			s << "000000" << content[i];
		}
		else if (content[i] < 10000)	{
			s << "00000" << content[i];
		}
		else if (content[i] < 100000)	{
			s << "0000" << content[i];
		}
		else if (content[i] < 1000000)	{
			s << "000" << content[i];
		}
		else if (content[i] < 10000000)	{
			s << "00" << content[i];
		}
		else if (content[i] < 100000000)	{
			s << "0" << content[i];
		}
		else	{
			s << content[i];
		}
	}
	return s.str();
}

BigInt BigInt :: operator = (BigInt n)	{
	sign = n.sign;
	length = n.length;
	memcpy(content, n.content, sizeof(content));
}

bool BigInt :: operator == (BigInt n)	{
	if (sign != n.sign)	{
		return false;
	}
	else if (length != n.length)	{
		return false;
	}
	else	{
		for (int i = 0; i < length; i ++)	{
			if (content[i] != n.content[i])	return false;
		}
		return true;
	}
}

bool BigInt :: operator != (BigInt n)	{
	return !operator == (n);
}

bool BigInt :: operator < (BigInt n)	{
	if (sign < n.sign)	{
		return true;
	}
	else if (sign > n.sign)	{
		return false;
	}
	else if (sign == 0)	{
		return false;
	}
	else if (sign == 1 && length < n.length)	{
		return true;
	}
	else if (sign == 1 && length > n.length)	{
		return false;
	}
	else if (sign == 1 && length == n.length)	{
		for (int i = 0; i < length; i ++)	{
			if (content[i] < n.content[i])	{
				return true;
			}
			else if (content[i] > n.content[i])	{
				return false;
			}
		}
		return false;
	}
	else if (sign == -1 && length < n.length)	{
		return false;
	}
	else if (sign == -1 && length > n.length)	{
		return true;
	}
	else if (sign == -1 && length == n.length)	{
		for (int i = 0; i < length; i ++)	{
			if (content[i] < n.content[i])	{
				return false;
			}
			else if (content[i] > n.content[i])	{
				return true;
			}
		}
		return false;
	}
}

bool BigInt :: operator <= (BigInt n)	{
	return (operator < (n) || operator == (n));
}

bool BigInt :: operator > (BigInt n)	{
	return !operator <= (n);
}

bool BigInt :: operator >= (BigInt n)	{
	return !operator < (n);
}

BigInt BigInt :: operator + (BigInt n)	{

	if (length == 0)	{
		return n;
	}
	else if (n.length == 0)	{
		BigInt num(*this);
		return num;
	}
	else if (sign == n.sign)	{
		BigInt num;
		num.length = (length > n.length) ? length : n.length;
		num.sign = sign;

		long long bucketSum, carry = 0;

		for (int i = 0; i < num.length; i ++)	{
			bucketSum = content[i] + n.content[i] + carry;
			num.content[i] = bucketSum % BUCKET_LIMIT;
			carry = bucketSum / BUCKET_LIMIT;
		}
		if (carry > 0)	{
			num.content[length ++] = carry;
		}
		return num;
	}
	else	{
		BigInt num;
		if (sign == -1)	{
			num = (*this);
			num.sign = 1;
			return n - num;
		}
		else	{
			num = n;
			num.sign = 1;
			return (*this) - num;
		}
	}
}

BigInt BigInt :: operator - (BigInt n)	{

	if (sign == n.sign)	{
		bool hasZeroValue = true;

		if (length == 0)	{
			BigInt subtrahend(n);
			subtrahend.sign *= -1;
		}
		else if (n.length == 0)	{
			BigInt num(*this);
			return num;
		}
		if (*this > n)	{
			BigInt minuend(*this);

			for (int i = 0; i < minuend.length; i ++)	{
				if (minuend.content[i] >= n.content[i])	{
					minuend.content[i] -= n.content[i];
				}
				else	{
					minuend.content[i] = 10 * minuend.content[i] - n.content[i];
					minuend.content[i + 1] --;
				}
				if (hasZeroValue && minuend.content[i] != 0)	hasZeroValue = false;
			}
			return hasZeroValue ? BigInt(0) : minuend;
		}
		else	{
			BigInt minuend(n);
			for (int i = 0; i < minuend.length; i ++)	{
				if (minuend.content[i] >= content[i])	{
					minuend.content[i] -= content[i];
				}
				else	{
					minuend.content[i] = 10 * n.content[i] - content[i];
					minuend.content[i + 1] --;
				}
				if (hasZeroValue && minuend.content[i] != 0)	hasZeroValue = false;
			}
			if (hasZeroValue)	{
				return BigInt(0);
			}
			else	{
				minuend.sign *= -1;
				return minuend;
			}
		}
	}
	else	{
		BigInt subtrahend(n);
		if (n.sign = -1)	{
			subtrahend.sign = 1;
		}
		else	{
			subtrahend.sign = -1;
		}
		return operator + (subtrahend);
	}
}

BigInt BigInt :: operator * (int n)	{
	if (n == 0 || length == 0)	return BigInt(0);

	long long carry = 0, tempProduct;
	BigInt num(*this);
	num.sign = ((n > 0 && sign == 1) || (n < 0 && sign == -1)) ? 1 : -1;

	if (n < 0)	n *= -1;

	for (int i = 0; i < length; i ++)	{
		tempProduct = num.content[i] * n;
		num.content[i] = (tempProduct + carry) % BUCKET_LIMIT;
		carry = (tempProduct + carry) / BUCKET_LIMIT;
	}

	if (carry > 0)	num.content[num.length ++] = carry;
	return num;
}

BigInt BigInt :: operator * (BigInt n)	{
	BigInt rs, temp;
	rs = n * content[0];

	for (int i = 1; i < length; i ++)	{
		temp = n * content[i];
		temp.addLSBucket(i);
		rs = rs + temp;
	}
	rs.sign = (sign == n.sign) ? 1 : -1;
	return rs;
}

void BigInt :: debug (void)	{
	cout << endl << "--------------------------------------------------" << endl;
	cout << "In debug mode" << endl;
	cout << "Number: " << toString() << endl;
	cout << "Sign: " << sign << "\tLength: " << length << endl;
	cout << "Content: ";
	for (int i = 0; i < MAX; i ++)	{
		cout << content[i] << "  ";
	}
	cout << endl << "--------------------------------------------------" << endl;
}

int main ()
{
	BigInt a("9999999999123456789123456"), b("12345678912"), c;
	c = a + b;
	cout << a.toString() + " + " + b.toString() + " = " + c.toString() << endl;
	c = a - b;
	cout << a.toString() + " - " + b.toString() + " = " + c.toString() << endl;
	c = a * b;
	cout << a.toString() + " * " + b.toString() + " = " + c.toString() << endl;

	if (a == b)	{
		cout << a.toString() + " is equal to " + b.toString() << endl;
	}
	else	{
		cout << a.toString() + " is not equal to " + b.toString() << endl;
	}

	if (a > b)	{
		cout << a.toString() + " is greater than " + b.toString() << endl;
	}
	else	{
		cout << a.toString() + " is not greater than " + b.toString() << endl;
	}
	return 0;
}
