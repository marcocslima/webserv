/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostMethod.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jefernan <jefernan@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 18:31:10 by jefernan          #+#    #+#             */
/*   Updated: 2023/10/13 16:47:39 by jefernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "AResponse.hpp"
# include "Request.hpp"
# include "Libs.hpp"

# define P_BLUE			"\e[0;38;5;24m"
# define RESET			"\e[0m"
class PostMethod : public AHttpResponse
{

    public:
        PostMethod();
        PostMethod(HttpRequest request);
        ~PostMethod();

        std::string     handleMethod(std::string uri);
        responseData    handlePost();
        void            handleMultipart();
        void            handleForm();
        void            print();
        void            setResponse(std::string statusCode, std::string content);
        void            saveFile(std::string& fileName, const std::string& value);
        void            parseMultipartFormData(size_t pos, size_t endPos);

        bool            created;
    private:
        HttpRequest                         _httpRequest;
        responseData                        _responseData;
        bool                                _file;
        std::map<std::string, std::string>  _formData;
};