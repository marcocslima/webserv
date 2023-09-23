/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostMethod.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jefernan <jefernan@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 18:31:10 by jefernan          #+#    #+#             */
/*   Updated: 2023/09/23 18:31:59 by jefernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "AResponse.hpp"

class PostMethod : public AHttpResponse {

    public:
        PostMethod();
        ~PostMethod();

    std::string handleMethod(std::string uri);
};