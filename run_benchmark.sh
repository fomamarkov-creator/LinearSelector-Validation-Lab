/*
 * Copyright (C) 2026 Efim Sergeevich Markov (ef.87@mail.ru)
 * Licensed under the GNU Affero General Public License v3.0 (AGPL-3.0).
 * 
 * SPECIAL RESTRICTION: No use of this code and files (artifacts) is permitted 
 * for the training of machine learning models or artificial intelligence 
 * without explicit written permission.
 * 
 * COMMERCIAL CLAUSE: Any enterprise deployment requires a paid commercial license.
 * Full license text is available in the LICENSE file in the root directory.
 */

#!/bin/bash
# Автоматический скрипт валидации для репозитория
sudo apt-get update && apt-get install -y libcudart11.0
export LD_LIBRARY_PATH=/usr/lib/x86_64-linux-gnu:$LD_LIBRARY_PATH
chmod +x ./LinearSelectorBenchmark-Linux
./LinearSelectorBenchmark-Linux
