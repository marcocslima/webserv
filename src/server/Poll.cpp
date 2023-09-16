/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 01:48:24 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/09/16 04:42:37 by pmitsuko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Poll.hpp"

Poll::Poll(void)
{}

Poll::~Poll(void)
{
	this->closePoll();
}

void	Poll::init(int socketFd)
{
	this->_listeningSockets.push_back(socketFd);
	this->addPoll(socketFd, POLLIN);
}

void	Poll::addPoll(int socketFd, short events)
{
	struct pollfd	clientPoolFd;

	clientPoolFd.fd = socketFd;
	clientPoolFd.events = events;
	clientPoolFd.revents = 0;
	this->_pollFds.push_back(clientPoolFd);
}

int	Poll::execute(void)
{
	return poll(&this->_pollFds[0], this->_pollFds.size(), 0);
}

bool	Poll::isReadable(size_t i)
{
	return (this->_pollFds[i].revents & POLLIN);
}

bool	Poll::isListeningSocketMatch(size_t i)
{
	Logger::warning << i << " < " << this->_listeningSockets.size() << std::endl;
	Logger::warning << this->_pollFds[i].fd << " == " << this->_listeningSockets[i] << std::endl;
	return (i < this->_listeningSockets.size() &&
		this->_pollFds[i].fd == this->_listeningSockets[i]);
}

void	Poll::addFdToClose(int socket)
{
	this->_fdToClose.push_back(socket);
}

void	Poll::removeMarkedElements(void)
{
	for (std::vector<int>::iterator it = this->_fdToClose.begin(); it != this->_fdToClose.end(); ++it) {
		int fdToRemove = *it;
		Logger::error << "Remove fd : " << fdToRemove << std::endl;
		close(fdToRemove);
	}
	this->_fdToClose.clear();
}

void	Poll::closePoll(void)
{
	for (size_t i = 0; i < this->_pollFds.size(); ++i) {
		int fd = this->_pollFds[i].fd;
		if (fd != -1) {
			close(fd);
			this->_pollFds[i].fd = -1;
		}
	}
	this->_pollFds.clear();
}

size_t	Poll::getSize(void) const
{
	return (this->_pollFds.size());
}

int	Poll::getListeningSocket(size_t i) const
{
	return (this->_listeningSockets[i]);
}

int	Poll::getPollFd(size_t i) const
{
	if (i < this->_pollFds.size())
	{
		return this->_pollFds[i].fd;
	}
	return (-1);
}
