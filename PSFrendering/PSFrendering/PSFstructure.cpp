#include "PSFrendering.h"

void loadPSFs(std::string& camera_path, std::vector<DepthDatabase>& psfsDict) {

    path p(camera_path);

    if (!exists(p) || !is_directory(p)) {
        std::cout << "ERROR: path error" << std::endl;
        return;
    }

    insertAllDepthFolders(p,  psfsDict);

	return;
}


void insertAllDepthFolders(path& p, std::vector<DepthDatabase>& depths) {

    try {

        for (directory_entry& dir : directory_iterator(p)) {

            //bisogna rimuovere il nome depth
            DepthDatabase new_depth(atof(dir.path().filename().generic_string().c_str()));

            for (directory_entry file : directory_iterator(dir)) {

                auto psfKer = cv::imread(file.path().generic_string(), cv::IMREAD_GRAYSCALE);
                double com_x = atof(splitString(file.path().stem().generic_string())[0].c_str());
                double com_y = atof(splitString(file.path().stem().generic_string())[1].c_str());

                PSF new_psf(com_x, com_y, psfKer);

                new_depth.m_psfs.push_back(new_psf);

                //PSF constructor do a kernel reference import, verify that outside the brackeys works
            }

            depths.push_back(new_depth);
        }

    } catch (const filesystem_error& ex)
    {
        std::cout << ex.what() << std::endl;

    }

    //working test

    for (DepthDatabase& depth : depths) {
        std::cout << depth.getDepth() << std::endl;
    }

    return;

}

std::vector<std::string> splitString(std::string str) {

    std::vector<std::string> tokens;
    std::string str_found = "";

    for (auto& x : str) {
      
        if (x == '-') {
            tokens.push_back(str_found);
            str_found = "";
            continue;
        }
            
        str_found += x;
    }

    tokens.push_back(str_found);
    return tokens;
}