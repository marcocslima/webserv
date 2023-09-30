/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostMethod.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jefernan <jefernan@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 18:31:10 by jefernan          #+#    #+#             */
/*   Updated: 2023/09/30 16:51:33 by jefernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "AResponse.hpp"
# include <iomanip>

# define P_BLUE			"\e[0;38;5;24m"
# define RESET			"\e[0m"
class PostMethod : public AHttpResponse
{

    public:
        PostMethod();
        PostMethod(HttpRequest request);
        ~PostMethod();

        std::string handleMethod(std::string uri);
        void        handleMultipart();
        void        handleForm();
        void        print();
        void        saveFile(size_t pos, const std::string& value, const std::string& data);
        void parseMultipartFormData(size_t pos, size_t endPos);

        bool    created;
    private:
        HttpRequest                         _httpRequest;
        std::string                         _contentType;
        std::map<std::string, std::string>  _formData;
};