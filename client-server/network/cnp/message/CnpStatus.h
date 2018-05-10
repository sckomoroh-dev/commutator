//
// Created by sckomoroh on 06.05.18.
//

#ifndef COMMUTATOR_CNPSTATUSES_H
#define COMMUTATOR_CNPSTATUSES_H

namespace network::cnp::message
{
    enum class CnpStatus
    {
		Unknown,
        StatusOk,
        StatusError
    };
}

#endif //COMMUTATOR_CNPSTATUSES_H
