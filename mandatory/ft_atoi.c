#include "philosopher.h"


int	ft_isdigit(char *s)
{
	while(*s)
    {
		if (*s >= '0' && *s <= '9')
			s++;
		else
		return (0);
    }
    return(1);
}


int	ft_atoi(const char *str)
{
	long long		res;
	int				sign;
	int				i;

	i = 0;
	res = 0;
	sign = 1;
	if (!*str)
		return (0);
    if(!ft_isdigit)
        return(-1);
	while (((((str[i] >= 9) && (str[i] <= 13))
				|| (str[i] == ' ')) && (str[i] != '\0')))
				i++;
	if (str[i] == '-')
		sign = -1;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + str[i] - '0';
		i++;
	}
	return ((sign * res));
}