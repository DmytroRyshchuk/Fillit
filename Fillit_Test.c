/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Fillit_Test.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dryshchu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/20 12:49:12 by dryshchu          #+#    #+#             */
/*   Updated: 2017/11/20 12:49:18 by dryshchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define BUF_SIZE 4096
#define CHECK (j + t->x[l_a] < size) &&\
	(i + t->y[l_a] < size) && (j + t->x[l_a] >= 0) &&\
	(i + t->y[l_a] >= 0) && (s[j + t->x[l_a]][i + t->y[l_a]] == '.')



void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putstr(char const *s)
{
	int		i;

	if (s == NULL)
		return ;
	i = -1;
	while (s[++i])
		ft_putchar(s[i]);
}

size_t	ft_strlen(const char *s)
{
	size_t			i;
	unsigned char	*st;

	st = (unsigned char*)s;
	i = 0;
	while (st[i])
		i++;
	return (i);
}






typedef	struct
{
	int x[4];
	int y[4];
} 		figure;




int		check_contacts_figure (char *s)
{
	int		i;
	int 	j;
	int 	contact;

	i = -1;
	j = 0;
	contact = 0;
	while (s[++i])
	{
		if ((s[i] == '\n') && ((s[i + 1] == '\n') || s[i + 1] == '\0'))
		{
			if (contact < 5)
				return (0);
			j = -2;
			contact = 0;
		}
		contact = (s[i] == '#' && s[i - 1] == '#') ? contact + 1 : contact;
		contact = (s[i] == '#' && s[i + 1] == '#') ? contact + 1 : contact;
		if (s[i] == '#' && j % 19 <= 13 && s[i + 5] == '#')
			contact++;
		if (s[i] == '#' && j % 19 >= 3 && s[i - 5] == '#')
			contact++;
		j++;
	}
	return (1);
}

//check to valid tetramino

int		check_if_figures_are_valid(char *s)
{
	int		i;
	int 	j;
	int tmp;

	j = 0;
	i = 0;
	tmp = 0;
	while (s[i])
	{
		if ((s[i] == '\n') && ((s[i + 1] == '\n') || s[i + 1] == '\0'))
			j = -2;
		if ((j % 19 >= 0 && j % 19 <= 3 && s[i] == '#') &&
			(s[i - 1] != '#' && s[i + 1] != '#' && s[i + 5] != '#'))
			return (0);
		else if ((j % 19 >= 15 && j % 19 <= 18 && s[i] == '#') &&
			(s[i - 1] != '#' && s[i + 1] != '#' && s[i - 5] != '#'))
			return (0);
		else if ((s[i] == '#') && (s[i - 1] != '#' && s[i + 1] != '#' &&
			s[i + 5] != '#' && s[i - 5] != '#'))
			return (0);
		i++;
		j++;
	}
	return (1);
}

//main check function

int		check_if_file_is_valid(char *str)
{
	int		i;
	int		sharp;
	int		dots;

	i = -1;
	sharp = 0;
	dots = 0;
	while (str[++i])
	{
		if ((str[i] == '\n') && ((str[i + 1] == '\n') || str[i + 1] == 0))
		{
			if (dots != 12 && sharp != 4)
				return (0);
			dots = 0;
			sharp = 0;
		}
		if (str[i] == '#')
			sharp++;
		else if (str[i] == '.')
			dots++;
		else if (str[i] != '\n')
			return (0);
	}
	return (check_if_figures_are_valid(str) == 0) || 
			(check_contacts_figure(str) == 0) || i == 0 ? 0 : 1;
}














char *read_file(char *str)
{
	char	buf[BUF_SIZE];
	char 	*p;
	int		fd;
	int		len;
	int		i;

	i = -1;
	fd = open(str, O_RDONLY);
	if (fd == -1)
		exit (0);
	len = read(fd, buf, BUF_SIZE);
	buf[len] = '\0';
	close(fd);
	p = (char*)malloc(sizeof(char) * (len + 1));
	if(p == NULL)
		return (NULL);
	while (i++ < len)
		p[i] = buf[i];
	p[i] = '\0';
	return (p);
}

figure	*write_one_figure(figure *tetramines, char *st)
{
	int		i;
	int		j;
	int		tmpx;
	int		tmpy;
	int		first;

	i = 0;
	j = 0;
	while (i != 20)
	{
		if (st[i] == '#')
		{
			if (first == 0)
			{
				tmpx = i / 5;
				tmpy = i % 5;
				first = 1;
			}
			tetramines->x[j] = i / 5 - tmpx;
			tetramines->y[j] = i % 5 - tmpy;
			j++;
		}
		i++;
	}
	return (tetramines);
}

figure	**write_figures(figure **tetramines, char *st)
{
	int		fig_num;
	int		i;
	int		j;

	fig_num = (ft_strlen(st) + 1) / 21;
	i = 0;
	j = 0;
	tetramines = (figure**)malloc(sizeof(figure*) * fig_num);
	while ((j < fig_num) && (st[i]))
	{
		tetramines[j] = (figure*)malloc(sizeof(figure));
		tetramines[j] = write_one_figure(tetramines[j], &st[i]);
		j++;
		i = i + 21;
	}
	return (tetramines);
}

int		size_of_side(char *st)
{
	int		side_size;
	int		count;

	side_size = 2;
	count = (ft_strlen(st) + 1) / 21;
	while (side_size * side_size < count * 4)
		side_size++;
	return (side_size);
}

char 	**make_square(char *str)
{
	int i;
	int tmp;
	char **square;
	
	i = -1;
	tmp = 0;
	square = (char**)malloc(sizeof(char*) * size_of_side(str));
	if (square == NULL)
		return (NULL);
	while (++i < size_of_side(str))
	{
		square[i] = (char*)malloc(sizeof(char*) * size_of_side(str));
		if (square[i] == NULL)
		{
			while (i-- > 0)
				free(square[i]);
			free(square);
			return (NULL);
		}
		while(tmp < size_of_side(str))
			square[i][tmp++] = '.';
		tmp = 0;
	}
	return (square);
}

//
int	fill(char **s, int j, int i, figure *t, int size)
{
	int l_a;

	l_a = 1;
	while (l_a < 4 && CHECK)
	{
		s[j + t->x[l_a]][i + t->y[l_a]] = s[j][i];
		l_a++;
	}
	if (l_a == 4)
		return (1);
	else
	{
		while(--l_a >= 1)
			s[j + t->x[l_a]][i + t->y[l_a]] = '.';
		s[j][i] = '.';
		return (0);
	}
}

// try to write algoritm
// s - square, size - size of side-square, tet - tetramino, count - number of tetrminos
char**	main_alg(char *str, figure *t, int count)
{
	int i = -1; // num of sumbol 
	int j = -1; // num of str 
	int l = 0; // 'A' + l == symbol of figure
	int l_a = 1; // address of sharp in tetramino 
	char **s;

	s = make_square(str);
	while (++j < size_of_side(str))
	{
		while(++i < size_of_side(str))
			if (s[j][i] == '.') // if square empty
			{
				s[j][i] = 'A' + l; // put char from tetramino 
				if (fill(s, j, i, t, size_of_side(str)) == 1)
				{
					l++;
					t++;
					i = 0;
					j = 0;
				}
			}
		i = 0;
	}
	return (s);
}

int		main(int ac, char **av)
{
	char	*st;
	figure	**Tetramines;
	int		size;
	char	**square;

	if (ac == 2)
	{
		st = read_file(av[1]);
		Tetramines = write_figures(Tetramines, st);

		ft_putstr(st);
		printf ("strlen - %zu\n", ft_strlen(st));
		printf ("count of figure = %zu\n", (ft_strlen(st) + 1) / 21);
		printf("result = %i\n", check_if_file_is_valid(st));
		size = size_of_side(st);
		square = main_alg(st, *Tetramines, (ft_strlen(st) + 1) / 21);

		int dot = 0;
		int str = 0;
		while (str < size)
		{
			while (dot < size)
			{
				printf(" %c ", square[str][dot]);
				dot++;
			}
			printf("\n");
			dot = 0;
			str++;
		}
	}
	return (0);
}