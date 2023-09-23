/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DeleteMethod.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcl <mcl@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 07:12:53 by mcl               #+#    #+#             */
/*   Updated: 2023/09/23 07:14:25 by mcl              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../includes/AResponse.hpp"

class DeleteMethod : public AHttpResponse {

    public:
        DeleteMethod();
        ~DeleteMethod();

    std::string handleMethod(std::string uri);
};
