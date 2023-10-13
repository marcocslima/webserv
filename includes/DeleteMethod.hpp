/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DeleteMethod.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 07:12:53 by mcl               #+#    #+#             */
/*   Updated: 2023/10/13 15:02:54 by pmitsuko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "AResponse.hpp"
#include "Constants.hpp"
#include "Utils.hpp"

class DeleteMethod : public AHttpResponse {
    public:
    DeleteMethod(void);
    DeleteMethod(HttpRequest request);
    ~DeleteMethod(void);

    responseData handleMethod(void);
};
