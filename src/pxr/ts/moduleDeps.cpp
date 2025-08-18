// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
////////////////////////////////////////////////////////////////////////

#include <pxr/ts/pxr.h>
#include <pxr/tf/registryManager.h>
#include <pxr/tf/scriptModuleLoader.h>
#include <pxr/tf/token.h>

#include <vector>

TS_NAMESPACE_OPEN_SCOPE

TF_REGISTRY_FUNCTION(TfScriptModuleLoader) {
    // List of direct dependencies for this library.
    const std::vector<TfToken> reqs = {
        TfToken("arch"),
        TfToken("tf"),
        TfToken("gf"),
        TfToken("vt"),
        TfToken("boost-python"),
    };
    TfScriptModuleLoader::GetInstance().
    RegisterLibrary(TfToken("ts"), TfToken("pxr.Ts"), reqs);
}

TS_NAMESPACE_CLOSE_SCOPE
