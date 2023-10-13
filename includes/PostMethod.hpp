/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostMethod.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 18:31:10 by jefernan          #+#    #+#             */
/*   Updated: 2023/10/13 17:05:34 by pmitsuko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "AResponse.hpp"

class PostMethod : public AHttpResponse {

    public:
    PostMethod();
    PostMethod(HttpRequest request);
    ~PostMethod();

    responseData handleMethod();
    void         handleMultipart();
    void         handleForm();
    void         print();
    void         saveFile(std::string &fileName, const std::string &value);
    void         parseMultipartFormData(size_t pos, size_t endPos);

    bool created;

    private:
    bool                               _file;
    std::map<std::string, std::string> _formData;
};
