/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DeleteMethod.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcl <mcl@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 07:12:53 by mcl               #+#    #+#             */
/*   Updated: 2023/10/05 10:56:30 by mcl              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "AResponse.hpp"
#include "Utils.hpp"

class DeleteMethod : public AHttpResponse {
    public:
        DeleteMethod();
        ~DeleteMethod();

    std::string handleMethod(std::string uri);
};
