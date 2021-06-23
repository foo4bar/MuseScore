/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-CLA-applies
 *
 * MuseScore
 * Music Composition & Notation
 *
 * Copyright (C) 2021 MuseScore BVBA and others
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef MU_WORKSPACE_WORKSPACEMANAGER_H
#define MU_WORKSPACE_WORKSPACEMANAGER_H

#include "../iworkspacemanager.h"
#include "modularity/ioc.h"
#include "async/asyncable.h"
#include "../iworkspaceconfiguration.h"
#include "../system/ifilesystem.h"
#include "workspace.h"
#include "extensions/iextensionsservice.h"

namespace mu::workspace {
class WorkspaceManager : public IWorkspaceManager, async::Asyncable
{
    INJECT(workspace, IWorkspaceConfiguration, configuration)
    INJECT(workspace, extensions::IExtensionsService, extensionsService)
    INJECT(workspace, system::IFileSystem, fileSystem)

public:
    void init();
    void deinit();

    RetValCh<IWorkspacePtr> currentWorkspace() const override;

    RetVal<IWorkspacePtrList> workspaces() const override;
    Ret setWorkspaces(const IWorkspacePtrList& workspaces) override;

private:
    void load();
    void saveCurrentWorkspace();

    Ret removeMissingWorkspaces(const IWorkspacePtrList& newWorkspaceList);
    Ret removeWorkspace(const IWorkspacePtr& workspace);
    bool canRemoveWorkspace(const std::string& workspaceName) const;

    Ret createInexistentWorkspaces(const IWorkspacePtrList& newWorkspaceList);
    Ret createWorkspace(IWorkspacePtr workspace);

    io::paths findWorkspaceFiles() const;
    void setupCurrentWorkspace();

    WorkspacePtr findByName(const std::string& name) const;
    WorkspacePtr findAndInit(const std::string& name) const;

    WorkspacePtr m_currentWorkspace;
    std::vector<WorkspacePtr> m_workspaces;

    async::Channel<IWorkspacePtr> m_currentWorkspaceChanged;
};
}

#endif // MU_WORKSPACE_WORKSPACEMANAGER_H