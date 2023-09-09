/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcl <mcl@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/01 03:21:54 by mcl               #+#    #+#             */
/*   Updated: 2023/09/09 00:46:24 by mcl              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/Parser.hpp"

using namespace std;

int main(int argc, char **argv) {

    Parser parser;

    if (argc != 2) {
        cout << "Error: Invalid number of arguments" << endl;
        exit(1);
    }

    parser.getConf(argv[1]);

    return 0;
}
