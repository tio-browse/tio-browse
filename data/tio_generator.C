#include <iostream>
#include <cstring>
#include "tio_generator_data.h"

void check(TIO_t err, const std::string message);
void writefile(fileinfo *info);
void writestate(TIO_File_t fileId, stateinfo *info);
void writemesh(TIO_File_t fileId, TIO_Object_t stateId, meshinfo *info);
void writematerial(TIO_File_t fileId, TIO_Object_t meshId, meshinfo *minfo,
                   matinfo *info);
void writequant(TIO_File_t fileId, TIO_Object_t meshId, meshinfo *minfo,
                quantinfo *info);

namespace
{
  bool debug;
}

int main(int argc, char *argv[])
{
  debug = false;

  for (int i = 0; i < argc; ++i)
  {
    if (strcmp("-d", argv[i]) == 0 || (strcmp ("--debug", argv[i]) == 0))
    {
      debug = true;
    }
  }

  for (size_t i = 0; i < sizeof(FILES) / sizeof(fileinfo); ++i)
  {
    writefile(&FILES[i]);
  }

  return EXIT_SUCCESS;
}

void check(TIO_t err, const std::string message)
{
  if (err != TIO_SUCCESS)
  {
    char errtext[TIO_STRLEN];

    std::cerr << message << ": " << err << " ";
    if (TIO_Get_Error(err, errtext) != TIO_SUCCESS)
    {
      std::cerr << "(*** Failed to get TyphonIO Error code ***)" << std::endl;
    }
    else
    {
      std::cerr << "(" << errtext << ")" << std::endl;
    }
    exit(EXIT_FAILURE);
  }
  if (debug) std::cerr << message << ": Success" << std::endl;
}

void writefile(fileinfo *info)
{
  TIO_File_t fileId;

  std::cout << "File: " << info->name << std::endl;
  check(TIO_Create(info->name, &fileId, TIO_ACC_REPLACE, "tio_generator",
                   info->version, info->date, info->title, MPI_COMM_NULL,
                   MPI_INFO_NULL, MPI_PROC_NULL), "TIO_Create");
  for (int s = 0; s < info->nstates; ++s)
  {
    writestate(fileId, &info->state[s]);
  }

  check(TIO_Close(fileId), "TIO_Close");
}

void writestate(TIO_File_t fileId, stateinfo *info)
{
  TIO_Object_t stateId;

  check(TIO_Create_State(fileId, info->name, &stateId, info->step, info->time,
                         info->units), "TIO_Create_State");
  for (int m = 0; m < info->nmeshes; ++m)
  {
    writemesh(fileId, stateId, &info->mesh[m]);
  }

  check(TIO_Close_State(fileId, stateId), "TIO_Close_State");
}

void writemesh(TIO_File_t fileId, TIO_Object_t stateId, meshinfo *info)
{
  TIO_Object_t meshId;

  check(TIO_Create_Mesh(fileId, stateId, info->name, &meshId, info->type,
                        info->coordtype, info->isAMR, info->group, info->order,
                        TIO_INT, TIO_DOUBLE, info->ndims, info->n1, info->n2,
                        info->n3, info->n4, info->nchunks, info->iunits,
                        info->junits, info->kunits, info->ilabel, info->jlabel,
                        info->klabel), "TIO_Create_Mesh");
  switch (info->type)
  {
    case TIO_MESH_QUAD_COLINEAR:
      for (TIO_Size_t c = 0; c < info->nchunks; ++c)
      {
        check(TIO_Set_Quad_Chunk(fileId, meshId, c, info->chunk[c].ndims,
                                 info->chunk[c].xl, info->chunk[c].xh,
                                 info->chunk[c].yl, info->chunk[c].yh,
                                 info->chunk[c].zl, info->chunk[c].zh,
                                 info->chunk[c].nmixcell,
                                 info->chunk[c].nmixcomp),
             "TIO_Set_Quad_Chunk");
      }
      //
      // Have to write entire mesh at once for Colinear Quad
      // - stored coordinates for entire mesh in 1st chunk
      //
      check(TIO_Write_QuadMesh_All(fileId, meshId, TIO_DOUBLE,info->chunk[0].x,
                                   info->chunk[0].y, info->chunk[0].z),
           "TIO_Write_QuadMesh_All");
      break;
    case TIO_MESH_QUAD_NONCOLINEAR:
      for (TIO_Size_t c = 0; c < info->nchunks; ++c)
      {
        check(TIO_Set_Quad_Chunk(fileId, meshId, c, info->chunk[c].ndims,
                                 info->chunk[c].xl, info->chunk[c].xh,
                                 info->chunk[c].yl, info->chunk[c].yh,
                                 info->chunk[c].zl, info->chunk[c].zh,
                                 info->chunk[c].nmixcell,
                                 info->chunk[c].nmixcomp),
             "TIO_Set_Quad_Chunk");
      }
      for (TIO_Size_t c = 0; c < info->nchunks; ++c)
      {
        check(TIO_Write_QuadMesh_Chunk(fileId, meshId, c, TIO_XFER_INDEPENDENT,
                                       TIO_DOUBLE, info->chunk[c].x,
                                       info->chunk[c].y, info->chunk[c].z),
             "TIO_Write_QuadMesh_Chunk");
      }
      break;
    case TIO_MESH_POINT:
      for (TIO_Size_t c = 0; c < info->nchunks; ++c)
      {
        check(TIO_Set_Point_Chunk(fileId, meshId, c, info->chunk[c].ndims,
                                 info->chunk[c].xl, info->chunk[c].xh,
                                 info->chunk[c].yl),
             "TIO_Set_Point_Chunk");
      }
      for (TIO_Size_t c = 0; c < info->nchunks; ++c)
      {
        check(TIO_Write_PointMesh_Chunk(fileId, meshId, c, TIO_XFER_INDEPENDENT,
                                        TIO_DOUBLE, info->chunk[c].x,
                                        info->chunk[c].y, info->chunk[c].z),
             "TIO_Write_PointMesh_Chunk");
      }
      break;
    default:
      std::cerr << "Unsupported mesh type" << std::endl;
      exit(EXIT_FAILURE);
  }

  if (info->material)
  {
    writematerial(fileId, meshId, info, info->material);
  }

  for (int q = 0; q < info->nquants; ++q)
  {
    writequant(fileId, meshId, info, &info->quant[q]);
  }

  check(TIO_Close_Mesh(fileId, meshId), "TIO_Close_Mesh");
}

void writequant(TIO_File_t fileId, TIO_Object_t meshId, meshinfo *minfo,
                quantinfo *info)
{
  TIO_Object_t quantId;

  check(TIO_Create_Quant(fileId, meshId, info->name, &quantId, TIO_DOUBLE,
                         info->centre, info->nghosts, info->ismixed,
                         info->units),
       "TIO_Create_Quant");
  switch (minfo->type)
  {
    case TIO_MESH_QUAD_COLINEAR:
    case TIO_MESH_QUAD_NONCOLINEAR: // fall through
      for (TIO_Size_t c = 0; c < info->nchunks; ++c)
      {
        check(TIO_Write_QuadQuant_Chunk(fileId, quantId, c,
                                        TIO_XFER_INDEPENDENT, TIO_DOUBLE,
                                        info->chunk[c].quant,
                                        info->chunk[c].mix),
             "TIO_Write_QuadQuant_Chunk");
      }
      break;
    case TIO_MESH_POINT:
      for (TIO_Size_t c = 0; c < info->nchunks; ++c)
      {
        check(TIO_Write_PointQuant_Chunk(fileId, quantId, c,
                                         TIO_XFER_INDEPENDENT, TIO_DOUBLE,
                                         info->chunk[c].quant),
             "TIO_Write_PointQuant_Chunk");
      }
      break;
    default:
      std::cerr << "Unsupported mesh type" << std::endl;
      exit(EXIT_FAILURE);
  }

  check(TIO_Close_Quant(fileId, quantId), "TIO_Close_Quant");
}

void writematerial(TIO_File_t fileId, TIO_Object_t meshId, meshinfo *minfo,
                   matinfo *info)
{
  TIO_Object_t matId;

  check(TIO_Create_Material(fileId, meshId, info->name, &matId, TIO_INT,
                            info->nmats, info->nghosts, info->ismixed,
                            info->ncomp_datatype, info->ptr_datatype,
                            info->vf_datatype),
       "TIO_Create_Material");

  if (info->matnames)
  {
    check(TIO_Set_Material_Range(fileId, matId, TIO_INT, info->nmats, NULL,
                                 info->matnames),
         "TIO_Set_Material_Range");
  }

  switch (minfo->type)
  {
    case TIO_MESH_QUAD_COLINEAR:
    case TIO_MESH_QUAD_NONCOLINEAR: // fall through
      for (TIO_Size_t c = 0; c < info->nchunks; ++c)
      {
        check(TIO_Write_QuadMaterial_Chunk(fileId, matId, c,
                                           TIO_XFER_INDEPENDENT, TIO_INT,
                                           info->chunk[c].mat,
                                           info->ncomp_datatype,
                                           info->ptr_datatype,
                                           info->vf_datatype,
                                           info->chunk[c].ncomp,
                                           info->chunk[c].ptr,
                                           info->chunk[c].mixmat,
                                           info->chunk[c].vf),
             "TIO_Write_QuadMaterial_Chunk");
      }
      break;
    case TIO_MESH_POINT:
      for (TIO_Size_t c = 0; c < info->nchunks; ++c)
      {
        check(TIO_Write_PointMaterial_Chunk(fileId, matId, c,
                                            TIO_XFER_INDEPENDENT, TIO_INT,
                                            info->chunk[c].mat),
             "TIO_Write_PointMaterial_Chunk");
      }
      break;
    default:
      std::cerr << "Unsupported mesh type" << std::endl;
      exit(EXIT_FAILURE);
  }

  check(TIO_Close_Material(fileId, matId), "TIO_Close_Material");
}
