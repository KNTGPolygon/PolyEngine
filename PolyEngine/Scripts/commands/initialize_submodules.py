import argparse
import os
import git
import logging
import sys
from enum import Enum

import common

def initialize_submodules(script_env):
    logger = script_env.get_logger(name='initialize_submodules')    
    logger.info('Using git repository location {}'.format(script_env.repo_path))

    repo = git.Repo(script_env.repo_path)
    repo_submodules = repo.submodules

    patches_folder = os.path.join(script_env.repo_path, 'PolyEngine', 'ThirdParty', 'patches')

    for submodule in repo_submodules:
        sub_name = os.path.basename(os.path.normpath(submodule.path))
        logger.info('Initializing submodule [{}] in path: {}'.format(sub_name, submodule.path))
        submodule.update(init=True, force=True)
        
        patch_name = os.path.join(patches_folder, '{}.patch'.format(sub_name))

        # Apply patch if needed
        if os.path.isfile(patch_name):
            logger.info('Applying patch to submodule {} found in {}'.format(sub_name,patch_name))
            sub_repo = submodule.module()
            sub_repo.git.reset(['--hard']) # Reset first
            sub_repo.git.apply([patch_name]) # Apply patch

def execute(script_env, *args):
    print('Using execute!')
    initialize_submodules(script_env)

if __name__ == '__main__':
    execute(common.SCRIPT_ENV, *sys.argv[1:])
