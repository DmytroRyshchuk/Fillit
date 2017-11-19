/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fillit.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omakovsk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/13 17:48:27 by omakovsk          #+#    #+#             */
/*   Updated: 2017/11/14 18:48:57 by omakovsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "libft/libft.h"
#define BUF_SIZE 4096

typedef struct
{
	int x[4];
	int y[4];
//	char letter;
} figure;

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

/*check to valid tetramino 
(if we have < 5 touches between sharps it's invalid figure)*/

int		check_contacts_figure (char *s)
{
	int		i;
	int 	j;
	int 	contact;

	i = 0;
	j = 0;
	contact = 0;
	while (s[i])
	{
		if ((s[i] == '\n') && ((s[i + 1] == '\n') || s[i + 1] == '\0'))
		{
			if (contact < 5)
				return (0);
			j = -2;
			contact = 0;
		}
		if (s[i] == '#' && s[i - 1] == '#')
			contact++;
		if (s[i] == '#' && s[i + 1] == '#')
			contact++;
		if (s[i] == '#' && j % 19 <= 13 && s[i + 5] == '#')
			contact++;
		if (s[i] == '#' && j % 19 >= 3 && s[i - 5] == '#')
			contact++;
		i++;
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

	i = 0;
	sharp = 0;
	dots = 0;
	while (str[i])
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
		i++;
	}
	return (check_if_figures_are_valid(str) == 0) || 
			(check_contacts_figure(str) == 0) || i == 0 ? 0 : 1;
}

//function that write coordinates of ONE tetramino to structure 

figure *write_one_tetr(figure *tetramino, char *str)
{
	int i;
	int tmpx;
	int tmpy;
	int k;
	int first;

	i = 0;
	k = 0;
	while (i != 20) // becouse we works with one block
	{
		if (str[i] == '#')
		{
			if (first == 0) // if first sharp in block 
			{
				tmpx = i / 5; //memorize first x-coordinate
				tmpy = i % 5; //memorize first y-coordinate
			} 
			tetramino->x[k] = i / 5 - tmpx; 
			tetramino->y[k] = i % 5 - tmpy;
			first++;
			k++;
		}
		i++;
	}
	return (tetramino);
}

/* function that allocate memory to each point 
to structure and each structute (like strsplit in libft) */

figure	**write_figures(figure **tetramino, char *str)
{
	int i;
	int j;
	size_t count;

	i = 0;
	j = 0;
	count = (ft_strlen(str) + 1) / 21;
	//allocate memory for pointers to structure
	tetramino = (figure**)malloc(sizeof(figure*) * count); 
	while((j < count) && (str[i]))
	{
		// allocate memory for each structure
		tetramino[j] = (figure*)malloc(sizeof(figure));

		//write in structure coordinates ONE tetramino with helps function WRITE_ONE_TETR
		tetramino[j] = write_one_tetr(tetramino[j], &str[i]); 
		j++;
		i = i + 21; /* after function write_one_tetr that works with one block 
						we jump to first element in next block;
					 	first block start - str[0], second block start - str[21], 
					 	third block - str[42], if the blocks end we jump excatly to '\0' symbol */
	}
	return (tetramino);
}	

//read from file 

char *read_file(char *str)
{
	char	buf[BUF_SIZE];
	char 	*p;
	int		fd;
	int		len;
	int		i;

	i = 0;
	fd = open(str, O_RDONLY);
	if (fd == -1)
		exit (0);
	len = read(fd, buf, BUF_SIZE);
	buf[len] = '\0';
	close(fd);
	p = (char*)malloc(sizeof(char) * (len + 1));
	if(p == NULL)
		return (NULL);
	while (i < len)
	{
		p[i] = buf[i];
		i++;
	}
	p[i] = '\0';
	return (p);
}

//count size of side-square

int		size_of_side(char *str)
{
	int size_side;
	int count;

	size_side = 2;
	count = (ft_strlen(str) + 1) / 21;
	while (size_side * size_side < count * 4)
	{
		size_side++;
	}
	return (size_side);
}

//make square 

char 	**make_square(char *str)
{
	int i;
	int tmp;
	char **square;
	int size_side;
	
	i = 0;
	tmp = 0;
	size_side = size_of_side(str);
	square = (char**)malloc(sizeof(char*) * size_side);
	if (square == NULL)
		return (NULL);
	while (i < size_side)
	{
		square[i] = (char*)malloc(sizeof(char*) * size_side);
		if (square[i] == NULL)
		{
			while (i-- > 0)
				free(square[i]);
			free(square);
			return (NULL);
		}
		while(tmp < size_side)
			square[i][tmp++] = '.';
		i++;
		tmp = 0;
	}
	return (square);
}

// try to write algoritm
// s - square, size - size of side-square, tet - tetramino, count - number of tetrminos

void	main_alg(char **s, int size, figure **t, int count)
{
	int i = 0; // num of sumbol 
	int j = 0; // num of str 
	int l = 0; // 'A' + l == symbol of figure
	int l_a = 1; // address of sharp in tetramino 

	while (j < size)
	{
		while(i < size)
		{
			if (s[j][i] == '.') // if square empty
			{
				s[j][i] = 'A' + l; // put char from tetramino 
				while ((l_a < 4) && 
					(j + t[l]->x[l_a] < size) && 
					(i + t[l]->y[l_a] < size) &&
					(j + t[l]->x[l_a] >= 0) && 
					(i + t[l]->y[l_a] >= 0) &&
					(s[j + t[l]->x[l_a]][i + t[l]->y[l_a]] == '.'))
				{
					s[j + t[l]->x[l_a]][i + t[l]->y[l_a]] = 'A' + l;
					l_a++;
				}
				if (l_a == 4)
					l++;
				else
				{
					while(--l_a >= 1)
					{
						s[j + t[l]->x[l_a]][i + t[l]->y[l_a]] = '.';
					}
					s[j][i] = '.';
				}
				l_a = 1;
			}
			if (l == count - 1)
				return ;
//			if ((j == size - 1) && (i == size - 1)) //&& (l < count))
//			{
//				return ;
//			}
			i++;
		}
		i = 0;
		j++;
	}
	if (j == size)
		return ;
}

int main (int argc, char **argv) 
{	
	char	*str;
	char 	**square;
	figure 	**TETRAMINO;
	int size; // size of square 

	if (argc == 2)
	{
		str = read_file(argv[1]);
		int count = ft_strlen(str) + 1 / 21; // num of tetraminos
		TETRAMINO = write_figures(TETRAMINO, str);

		ft_putstr(str);
		printf ("strlen - %zu\n", ft_strlen(str));
		printf ("count of figure = %zu\n", (ft_strlen(str) + 1) / 21);
		printf("result = %i\n", check_if_file_is_valid(str));
		size = size_of_side(str);
		square = make_square(str);
		main_alg(square, size, TETRAMINO, count);

// print square 5x5 but we have make function to print all of squares
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
		//print coordinate 16-th tetramino
		printf("TETRAMINO[2] \n %d ; %d\n %d ; %d\n %d ; %d\n %d ; %d\n",   TETRAMINO[2]->x[0], TETRAMINO[2]->y[0], 
																			TETRAMINO[2]->x[1], TETRAMINO[2]->y[1], 
																			TETRAMINO[2]->x[2], TETRAMINO[2]->y[2], 	
																			TETRAMINO[2]->x[3], TETRAMINO[2]->y[3]);
	}
}

//2 	....-#...-##..-.#..--

//3 	....-..#.-..##-..#.--

//4 	....-.##.-.##.-....--

//5 	.##.-##..-....-....--

//6 	..#.-..#.-..#.-..#.--

//7 	....-....-..##-.##.-
