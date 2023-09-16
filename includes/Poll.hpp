/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 01:48:07 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/09/16 04:58:01 by pmitsuko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef POLL_H
# define POLL_H

# include <vector>
# include <poll.h>
# include <unistd.h>

# include "Logger.hpp"

# include <algorithm>


class Poll
{
	private:
		std::vector<int>			_listeningSockets;
		std::vector<struct pollfd>	_pollFds;
		std::vector<int>			_fdToClose;

	public:
		Poll(void);
		~Poll(void);

		void	init(int socketFd);
		void	addPoll(int socketFd, short events);
		int		execute(void);
		bool	isReadable(size_t i);
		bool	isListeningSocketMatch(size_t i);
		void	addFdToClose(int fd);
		void	removeMarkedElements(void);
		void	closePoll(void);

		void	removeListeningSocket(int socketFd);
		void	removePollFd(int socketFd);

		size_t	getSize(void) const;
		int		getListeningSocket(size_t i) const;
		int		getPollFd(size_t i) const;
};

#endif
