/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostMethod.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jefernan <jefernan@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 18:31:10 by jefernan          #+#    #+#             */
/*   Updated: 2023/09/27 15:35:14 by jefernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "AResponse.hpp"

class PostMethod : public AHttpResponse {

    public:
        PostMethod();
        PostMethod(HttpRequest request);
        ~PostMethod();

        std::string	contentType;
        std::size_t	contentLength;
        std::string	transferEncoding;
        std::string	boundary;
        std::string	body;

        std::string handleMethod(std::string uri);
        bool        getContentType();
        void        getBody(std::string request);
        void        handleMultipart();
        void        handleForm();
        std::map<std::string, std::string> parseMultipartFormData(std::map<std::string, std::string> formData, size_t pos, size_t endPos);

    private:
        HttpRequest _httpRequest;
};