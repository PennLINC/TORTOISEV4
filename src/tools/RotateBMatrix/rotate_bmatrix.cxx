#ifndef _ROTATE_BMATRIX_CXX
#define _ROTATE_BMATRIX_CXX


#include "rotate_bmatrix.h"

vnl_vector<double> RotateBMatrixVec(vnl_vector<double> Bmatrixvec, const vnl_matrix_fixed<double,3,3> &rotmat)
{
    vnl_matrix<double> curr_Bmat(3,3);

    curr_Bmat(0,0)=Bmatrixvec[0];
    curr_Bmat(0,1)=Bmatrixvec[1]/2.;
    curr_Bmat(1,0)=Bmatrixvec[1]/2.;
    curr_Bmat(0,2)=Bmatrixvec[2]/2.;
    curr_Bmat(2,0)=Bmatrixvec[2]/2.;
    curr_Bmat(1,1)=Bmatrixvec[3];
    curr_Bmat(2,1)=Bmatrixvec[4]/2.;
    curr_Bmat(1,2)=Bmatrixvec[4]/2.;
    curr_Bmat(2,2)=Bmatrixvec[5];

    vnl_matrix<double> rotated_curr_Bmat = rotmat.transpose()  * curr_Bmat  * rotmat;

    vnl_vector<double> rot_bmat_vec(6);
    rot_bmat_vec[0]= rotated_curr_Bmat(0,0);
    rot_bmat_vec[1]= 2*rotated_curr_Bmat(0,1);
    rot_bmat_vec[2]= 2*rotated_curr_Bmat(0,2);
    rot_bmat_vec[3]= rotated_curr_Bmat(1,1);
    rot_bmat_vec[4]= 2*rotated_curr_Bmat(1,2);
    rot_bmat_vec[5]= rotated_curr_Bmat(2,2);

    return rot_bmat_vec;



}


vnl_matrix<double> RotateBMatrix(const vnl_matrix<double> &Bmatrix, const vnl_matrix_fixed<double,3,3> &rotmat, const vnl_matrix_fixed<double,3,3> &fixed_dirmat,const vnl_matrix_fixed<double,3,3> &moving_dirmat)
{
    //vnl_matrix_fixed<double,3,3> rotmat2= dirmat.transpose()*rotmat*dirmat;
    vnl_matrix_fixed<double,3,3> rotmat2= moving_dirmat.transpose() * rotmat * fixed_dirmat;

    int Nvols = Bmatrix.rows();

    vnl_matrix<double> new_Bmat(Nvols,6);
    for(int v=0;v<Nvols;v++)
    {
        vnl_vector<double> new_bmat_vec = RotateBMatrixVec(Bmatrix.get_row(v),rotmat2);
        new_Bmat.set_row(v,new_bmat_vec);
    }
    return new_Bmat;
}




vnl_matrix<double> RotateBMatrix(vnl_matrix<double> Bmatrix, std::vector<OkanQuadraticTransformType::Pointer> transforms,const vnl_matrix_fixed<double,3,3> &fixed_dirmat,const vnl_matrix_fixed<double,3,3> &moving_dirmat)
{
    int Nvols=Bmatrix.rows();
    vnl_matrix<double> new_Bmat(Nvols,6);


    for(int v=0;v<Nvols;v++)
    {
        OkanQuadraticTransformType::Pointer quad_trans= transforms[v];
        vnl_matrix_fixed<double,3,3> rot_mat=quad_trans->GetMatrix().GetVnlMatrix();

        //vnl_matrix_fixed<double,3,3> rotmat2= dirmat.transpose()*rot_mat*dirmat;
        vnl_matrix_fixed<double,3,3> rotmat2= moving_dirmat.transpose() * rot_mat * fixed_dirmat;
        vnl_vector<double> new_bmat_vec = RotateBMatrixVec(Bmatrix.get_row(v),rotmat2);
        new_Bmat.set_row(v,new_bmat_vec);
    }
    return new_Bmat;
}




vnl_matrix<double> RotateBMatrix(vnl_matrix<double> Bmatrix, std::vector<CompositeTransformType::Pointer> transforms,const vnl_matrix_fixed<double,3,3> &fixed_dirmat,const vnl_matrix_fixed<double,3,3> &moving_dirmat)
{
    int Nvols=Bmatrix.rows();
    vnl_matrix<double> new_Bmat(Nvols,6);


    for(int i=0;i<Nvols;i++)
    {
        vnl_matrix_fixed<double,3,3> rot_mat;
        rot_mat.set_identity();

        int ntrans= transforms[i]->GetNumberOfTransforms () ;
        for(int n=0;n<ntrans;n++)
        {
            OkanQuadraticTransformType::Pointer quad_trans= dynamic_cast< OkanQuadraticTransformType * > (transforms[i]->GetNthTransform(n).GetPointer());

            if(quad_trans)
            {
                rot_mat=  quad_trans->GetMatrix().GetTranspose() * rot_mat;
            }
        }
        rot_mat=rot_mat.transpose();

        //vnl_matrix_fixed<double,3,3> rotmat2= dirmat.transpose()*rot_mat*dirmat;
        vnl_matrix_fixed<double,3,3> rotmat2= moving_dirmat.transpose() * rot_mat * fixed_dirmat;

        vnl_vector<double> new_bmat_vec = RotateBMatrixVec(Bmatrix.get_row(i),rotmat2);
        new_Bmat.set_row(i,new_bmat_vec);
    }
    return new_Bmat;
}



#endif

