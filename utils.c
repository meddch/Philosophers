#include "philosopher.h"

static int	len_count(int n)
{
	int				l;
	unsigned int	nb;

	nb = n;
	l = 0;
	if (n < 0)
	{
		l++;
		nb = -n;
	}
	while (nb >= 10)
	{
			nb /= 10;
			l++;
	}
	return (l);
}

char	*ft_strdup( const char *source )
{
	char	*ptr;
	int		len ;
	int		i;

	i = 0;
	while (source[i] != '\0')
		i++;
	ptr = (char *)malloc((i + 1) * sizeof(char));
	len = i ;
	if (ptr == NULL)
		return (NULL);
	while (i)
	{
		*ptr++ = *source++;
		i--;
	}
	*ptr = '\0';
	return (ptr - len);
}

char	*ft_itoa(int n)
{
	char				*res;
	int					i;
	unsigned int		nb;

	i = 0;
	nb = n;
	i = len_count(n) + 1;
	if (n == 0)
		return (ft_strdup("0"));
	res = (char *)malloc((i + 1) * sizeof(char));
	if (!res)
		return (NULL);
	res[i--] = '\0';
	if (n < 0)
	{
		res[0] = 45;
		nb = -n ;
	}
	while (nb > 0)
	{
		res[i] = (nb % 10) + '0';
		nb /= 10;
		i--;
	}
	return (res);
}