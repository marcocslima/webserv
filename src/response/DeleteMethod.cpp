/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DeleteMethod.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 07:17:35 by mcl               #+#    #+#             */
/*   Updated: 2023/10/13 15:43:00 by pmitsuko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DeleteMethod.hpp"

DeleteMethod::DeleteMethod(void) {}

DeleteMethod::DeleteMethod(HttpRequest request) : AHttpResponse(request) {}

DeleteMethod::~DeleteMethod(void) {}

responseData DeleteMethod::handleMethod(void)
{

    std::string resourcePath = getDir();

    resourcePath = resourcePath + this->_req.getUri();

    Logger::warning << "resourcePath: " << resourcePath << std::endl;

    std::ifstream file(resourcePath.c_str());

    if (file.is_open()) {
        file.close();
        if (std::remove(resourcePath.c_str()) == 0) {
            this->_res = setResponseData(204, "", "", 0);
            Logger::info << "Resource deleted successfully." << std::endl;
        } else {
            this->_res = this->_errorPage.getErrorPageContent(
                this->_req.getErrorPageConfig(), "500", this->_req.getUri(), this->_req.getRoot());
            Logger::info << "Error deleting resource." << std::endl;
        }
    } else {
        this->_res = this->_errorPage.getErrorPageContent(
            this->_req.getErrorPageConfig(), "404", this->_req.getUri(), this->_req.getRoot());
        Logger::info << "Resource not found." << std::endl;
    }
    return (this->_res);
}
