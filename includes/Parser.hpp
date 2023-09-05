/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcl <mcl@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 03:18:36 by mcl               #+#    #+#             */
/*   Updated: 2023/09/05 03:57:34 by mcl              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <map>
#include <fstream>
#include <cstdlib>

class Parser
{
    private:
        std::string _conf;
        std::multimap<std::string, std::string> _data;
    public:
        Parser();
        ~Parser();
        Parser(const Parser& cpy);
        Parser& operator=(const Parser& obj);

        void	getData(const std::string& conf);
};