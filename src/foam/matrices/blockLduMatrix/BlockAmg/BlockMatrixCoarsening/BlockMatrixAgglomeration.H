/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | foam-extend: Open Source CFD
   \\    /   O peration     |
    \\  /    A nd           | For copyright notice see file Copyright
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of foam-extend.

    foam-extend is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    foam-extend is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with foam-extend.  If not, see <http://www.gnu.org/licenses/>.

Class
    BlockMatrixAgglomeration

Description
    Agglomerative block matrix AMG coarsening

Author
    Klas Jareteg, 2013-04-15

SourceFiles
    BlockMatrixAgglomeration.C

\*---------------------------------------------------------------------------*/

#ifndef BlockMatrixAgglomeration_H
#define BlockMatrixAgglomeration_H

#include "BlockMatrixCoarsening.H"
#include "BlockLduMatrix.H"
#include "BlockCoeffNorm.H"
#include "BlockCoeff.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

/*---------------------------------------------------------------------------*\
                           Class BlockMatrixAgglomeration Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class BlockMatrixAgglomeration
:
    public BlockMatrixCoarsening<Type>
{
    // Private Data

        //- Reference to matrix
        const BlockLduMatrix<Type>& matrix_;

        //- Reference to a templated norm calculator
        autoPtr<BlockCoeffNorm<Type> > normPtr_;

        //- Child array: for each fine equation give agglomeration cluster
        //  index
        labelField agglomIndex_;

        //- Group size
        label groupSize_;

        //- Number of coarse equations
        label nCoarseEqns_;

        //- Can a coarse level be constructed?
        bool coarsen_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        BlockMatrixAgglomeration(const BlockMatrixAgglomeration<Type>&);

        // Disallow default bitwise assignment
        void operator=(const BlockMatrixAgglomeration<Type>&);

        //- Calculate agglomeration index (child)
        void calcAgglomeration();


    // Private Static Data

        //- Diagonal scaling factor
        static const scalar diagFactor_;

        //- Weighting factor
        static const scalar weightFactor_;


public:

    //- Runtime type information
    TypeName("AAMG");


    // Constructors

        //- Construct from matrix and group size
        BlockMatrixAgglomeration
        (
            const BlockLduMatrix<Type>& matrix,
            const dictionary& dict,
            const label groupSize,
            const label minCoarseEqns
        );


    // Destructor

        virtual ~BlockMatrixAgglomeration();


    // Member Functions

        //- Can a coarse level be constructed?
        virtual bool coarsen() const
        {
            return coarsen_;
        }

        //- Restrict matrix
        virtual autoPtr<BlockLduMatrix<Type> > restrictMatrix() const;

        //- Restrict residual
        virtual void restrictResidual
        (
            const Field<Type>& res,
            Field<Type>& coarseRes
        ) const;

        //- Restrict CoeffField.  Used for diag coefficient
        void restrictDiag
        (
            const CoeffField<Type>& Coeff,
            CoeffField<Type>& coarseCoeff
        ) const;

        //- Prolongate correction
        virtual void prolongateCorrection
        (
            Field<Type>& x,
            const Field<Type>& coarseX
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "BlockMatrixAgglomeration.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
