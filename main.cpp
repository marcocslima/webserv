/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcl <mcl@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/01 03:21:54 by mcl               #+#    #+#             */
/*   Updated: 2023/09/16 00:09:52 by mcl              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/Parser.hpp"

using namespace std;

int main(int argc, char **argv) {


    if (argc != 2) {
        cout << "Error: Invalid number of arguments" << endl;
        exit(1);
    }

    Parser parser(argv[1]);

    parser.getServerParam(0, "listen");
    parser.getLocationParam(0, 0, "error_page");

    return 0;
}
