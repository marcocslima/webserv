/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jefernan <jefernan@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 13:18:47 by jefernan          #+#    #+#             */
/*   Updated: 2023/09/21 08:52:19 by jefernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "Logger.hpp"
# include "Server.hpp"
# include "Request.hpp"

class HttpResponse {
    private:
        HttpRequest _request;
    public:
        HttpResponse();
        ~HttpResponse();

    void	post(char buffer[1024]);
};