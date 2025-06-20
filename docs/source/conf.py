# SPDX-License-Identifier: LGPL-3.0-or-later
# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information
import datetime
import logging
import os
import sys

sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), "_scripts")))
from generate_bibtex import get_latest_jupedsim_bibtex

project = "JuPedSim"
copyright = (
    f"{datetime.datetime.today().year}, Forschungszentrum Jülich GmbH, IAS-7"
)

import jupedsim

version = "v" + jupedsim.__version__

logging.info(f"Create documentation for JuPedSim {version}")

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = [
    "sphinx_copybutton",
    "sphinx.ext.mathjax",
    "sphinx.ext.viewcode",
    "autoapi.extension",
    "sphinx_favicon",
    "notfound.extension",
    "sphinx.ext.mathjax",
    "myst_nb",
    "sphinx.ext.intersphinx",
    "sphinx.ext.napoleon",
    "sphinx.ext.autosectionlabel",
]

templates_path = ["_templates"]
exclude_patterns = []

# -- Linking ---------------------------------------------------------
intersphinx_mapping = {
    "python": ("https://docs.python.org/3/", None),
    "sphinx": ("https://www.sphinx-doc.org/en/master/", None),
    "shapely": ("https://shapely.readthedocs.io/en/2.0.1/", None),
}

# -- Automatic generation of API doc -----------------------------------------
autoapi_dirs = [
    "../../python_modules/jupedsim/jupedsim",
]
autoapi_root = "api"
autoapi_options = [
    "members",
    "undoc-members",
    "show-inheritance",
    "show-module-summary",
    "imported-members",
]
autoapi_ignore = [
    "**/tests/**",
    "**/native/**",
    "**/internal/**",
]
autoapi_add_toctree_entry = False
autoapi_python_class_content = "class"
autoapi_template_dir = "_templates/autoapi"
autoapi_member_order = "groupwise"
autoapi_python_use_implicit_namespaces = True

add_module_names = False

def skip_rules(app, what, name, obj, skip, options):
    if what == "module":
        skip = True
    if what == "method":
        if name.endswith("as_native"):
            skip = True
        if "tracing" in name:
            skip = True
        if "get_last_trace" in name:
            skip = True
    return skip


def setup(sphinx):
    sphinx.connect("autoapi-skip-member", skip_rules)

# -- Automatic execution of jupyter notebooks --------------------------------
nb_execution_excludepatterns = []
nb_execution_timeout = 900
myst_enable_extensions = [
    "amsmath",
    "colon_fence",
    "deflist",
    "dollarmath",
    "html_image",
]

nb_execution_raise_on_error = True

# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = "sphinx_book_theme"
html_static_path = ["_static"]

html_logo = "_static/jupedsim.svg"
favicons = [
    "logo.png",
]
html_css_files = [
    "css/custom.css",
]
html_js_files = [
    "https://cdnjs.cloudflare.com/ajax/libs/require.js/2.3.4/require.min.js",
]
html_context = {"default_mode": "light"}

html_theme_options = {
    "home_page_in_toc": False,
    "use_fullscreen_button": False,
    "use_issues_button": False,
    "use_download_button": False,
    "article_header_end": ["search-button", "toggle-secondary-sidebar"],
    "icon_links": [
        {
            "name": "GitHub",
            "url": "https://github.com/PedestrianDynamics/jupedsim",
            "icon": "fa-brands fa-github",
        },
        {
            "name": "PyPI",
            "url": "https://pypi.org/project/jupedsim/",
            "icon": "https://img.shields.io/pypi/v/jupedsim",
            "type": "url",
        },
        {
            "name": "DOI",
            "url": "https://doi.org/10.5281/zenodo.1293771",
            "icon": "https://zenodo.org/badge/DOI/10.5281/zenodo.1293771.svg",
            "type": "url",
        },
    ],
    "switcher": {
        "json_url": "https://www.jupedsim.org/versions.json",
        "version_match": version,
    },
    "primary_sidebar_end": ["version-switcher"],
    "show_toc_level": 2,
}

html_sidebars = {
    "**": ["navbar-logo", "icon-links", "search-field", "sbt-sidebar-nav.html"]
}

# -- Options for EPUB output
epub_show_urls = "footnote"

# -- Automatic fetch citation info from zenodo --------------------------------
bibtex = get_latest_jupedsim_bibtex(version)
logging.info(f"Bibtex fetched successfully:\n{bibtex}")
output_file = "citation/jupedsim_bibtex.bib"

with open(output_file, "w") as f:
    f.write(bibtex)
