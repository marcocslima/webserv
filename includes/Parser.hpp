/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcl <mcl@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 03:18:36 by mcl               #+#    #+#             */
/*   Updated: 2023/09/07 09:12:35 by mcl              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <map>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <sstream>

class Parser
{
    private:
        
    public:
        Parser();
        ~Parser();
        //Parser(const Parser& cpy);
        //Parser& operator=(const Parser& obj);

        void getFile(const char* fileconf);       
};