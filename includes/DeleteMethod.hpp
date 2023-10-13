/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DeleteMethod.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcl <mcl@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 07:12:53 by mcl               #+#    #+#             */
/*   Updated: 2023/10/13 15:38:04 by mcl              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "AResponse.hpp"
#include "utils.hpp"

class DeleteMethod : public AHttpResponse {
    public:
    DeleteMethod();
    ~DeleteMethod();

    std::string handleMethod(std::string uri);
};
