/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entry.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnouchet <mnouchet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 17:25:56 by mnouchet          #+#    #+#             */
/*   Updated: 2023/04/01 13:07:40 by mnouchet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENTRY_H
# define ENTRY_H

# define ENTRY 0
# define ENTRY_ERROR 1
# define HERE_DOC 2

int	entry(int *entries[2], int argc, char **argv);

#endif
