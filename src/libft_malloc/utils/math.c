long getNextMultiple(long n, long submultiple)
{
	long remainder = n % submultiple;
	if (remainder != 0) {
		return n - remainder + submultiple;
	} else {
		return n;
	}
}
