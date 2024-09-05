#ifndef DYNAMICUTILS_HPP
# define DYNAMICUTILS_HPP
# include <string>

size_t	oversize0Number(unsigned char *data, size_t size);
void	complement_memcpy(void *dest, const void *src, const size_t size);
void	complement(void *dest, const size_t size);
void	allocData(unsigned char *&data, size_t size); 
void	reallocData(unsigned char *&data, size_t &size, size_t newsize); 
void	memshift(unsigned char *buf, size_t len); 
void	printBinary(unsigned char *data, size_t len); 
size_t	floor(long double nbr); 
size_t	roundUp(size_t numToRound, size_t multiple); 
void	charToBinary(std::string &str, unsigned char *data, uint64_t size); 
void	memshiftL(unsigned char *data, size_t size, size_t shiftSize); 
bool	addToBuffer(unsigned char *data, size_t &size, unsigned char *add, size_t add_size, bool has_negative, bool fill_with_1); 
char	binaryAddition(unsigned char &dest, unsigned char &op, char buff);

#endif